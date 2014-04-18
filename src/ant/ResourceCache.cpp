#include <ant/resources/ResourceCache.hpp>
#include <ant/resources/ResourceLoaders.hpp>
#include <ant/resources/IResourceFile.hpp>
#include <ant/resources/ResourceFiles.hpp>
#include <ant/resources/ResourceHandle.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/gccUtils/String.hpp>
#include <ant/utils/DataUtils.hpp>
#include <cctype>			// for std::tolower

using namespace ant;

ant::ResourceCache::ResourceCache( const unsigned int sizeInMb, IResourceFile *file )
{
	m_cacheSize = dataUtils::convertMB2Byte(sizeInMb); // total memory size
	m_allocated = 0;														// total memory allocated
	m_file = file;
}

ant::ResourceCache::~ResourceCache()
{
	while (!m_lruResources.empty())
	{
		freeOneResource();
	}
}

bool ant::ResourceCache::init()
{
	bool retVal = false;
	if (m_file->open())
	{
		registerLoader(IResourceLoaderStrongPtr(GCC_NEW DefaultResourceLoader()));
		retVal = true;
	}
	return retVal;
}

void ant::ResourceCache::registerLoader( IResourceLoaderStrongPtr loader )
{
	m_resourceLoaderMap.push_front(loader);
}

ResourceHandleStrongPtr ant::ResourceCache::getResourceHandle( Resource *r )
{
	ResourceHandleStrongPtr handle(find(r));

	if (handle == NULL)
	{
		handle = load(r);
		GCC_ASSERT(handle);
	}
	else
	{
		updateLRU(handle);
	}
	return handle;
}

ant::ResourceHandleStrongPtr ant::ResourceCache::load( Resource *r )
{

	IResourceLoaderStrongPtr loader;
	ResourceHandleStrongPtr handle;

	// Find the loader based on the file ending of the resource identifier
	for (ResourceLoaders::iterator it = m_resourceLoaderMap.begin() ; it != m_resourceLoaderMap.end() ; ++it)
	{
		IResourceLoaderStrongPtr tloader = *it;
		if (WildcardMatch(tloader->getPattern().c_str(),r->getName().c_str()))
		{
			loader = tloader;
			break;
		}
	}

	// Check the loader
	if (!loader)
	{
		GCC_ASSERT(loader && _T("Default resource loader not found"));
		return handle;
	}

	// Try to get the size of the resource
	int rawSize = m_file->getRawResourceSize(*r);
	if (rawSize < 0)
	{
		std::string errormsg = "Resource size returned -1 - Resource: " + r->getName() + " is not found?";
		GCC_ASSERT(rawSize > 0 && errormsg.c_str());
		return ResourceHandleStrongPtr();
	}

	// Allocate memory
	int allocSize = rawSize + ((loader->addNULLZero()) ? (1) : (0));
	char *rawBuffer = loader->useRawFile() ? allocateMemoery(allocSize) : GCC_NEW char[allocSize];
	memset(rawBuffer, 0, allocSize);

	if (rawBuffer==NULL || m_file->getRawResource(*r, rawBuffer)==0)
	{
		// resource cache out of memory
		return ResourceHandleStrongPtr();
	}

	char* buffer = NULL;
	if (loader->useRawFile())
	{
		buffer = rawBuffer;
		handle = ResourceHandleStrongPtr(GCC_NEW ResourceHandle(*r,buffer,rawSize,this));
	}
	else
	{
		ant::UInt size = 0;
		size = loader->getLoadedResourceSize(rawBuffer,rawSize);
		buffer = allocateMemoery(size);
		if (rawBuffer==NULL ||buffer==NULL)
		{
			// The resource cache is out of memory
			return ResourceHandleStrongPtr();
		}
		handle = ResourceHandleStrongPtr(GCC_NEW ResourceHandle(*r,buffer,size,this));
		bool success = loader->loadResource(rawBuffer,rawSize,handle);

		/**
		 *  If the format is converted to a usable format, we need to remove the it since
		 *  it will take extra space
		 */
		if (loader->discardRawBufferAfterLoad())
		{
			SAFE_DELETE_ARRAY(rawBuffer);
		}

		if (!success)
		{
			// The cache is out off memory
			return ResourceHandleStrongPtr();
		}
	}
	// Insert the new handle in the list
	if (handle)
	{
		m_lruResources.push_front(handle);
		m_resourceHandleMap[r->getName()] = handle;
	}

	GCC_ASSERT(loader && _T("Default resource loader not found"));
	return handle;
}

int ant::ResourceCache::preLoad( const std::string pattern, void (*progressCallback)(int,bool &) )
{
	if (m_file == NULL)
	{
		return 0;
	}

	int numFiles = m_file->getNumResources();
	int loaded = 0;
	bool cancel = false;
	for (int i=0 ; i < numFiles ; i++)
	{
		Resource res(m_file->getResourceName(i));

		if (WildcardMatch(pattern.c_str(),res.getName().c_str()))
		{
			ResourceHandleStrongPtr handle = getResourceHandle(&res);
		}

		if ( progressCallback != NULL )
		{
			progressCallback(i * 100/numFiles, cancel);
		}
	}
	return loaded;
}

std::vector<std::string> ant::ResourceCache::match( const std::string pattern )
{
	std::vector<std::string> matchingNames;
	if (m_file==NULL)
		return matchingNames;

	int numFiles = m_file->getNumResources();
	for (int i=0; i<numFiles; ++i)
	{
		std::string name = m_file->getResourceName(i);
		std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
		if (WildcardMatch(pattern.c_str(), name.c_str()))
		{
			matchingNames.push_back(name);
		}
	}
	return matchingNames;
}

void ant::ResourceCache::flush()
{
	while (!m_lruResources.empty())
	{
		ResourceHandleStrongPtr handle = *(m_lruResources.begin());
		free(handle);		
	}
}

ant::ResourceHandleStrongPtr ant::ResourceCache::find( Resource *r )
{
	ResourceHandleMap::iterator it = m_resourceHandleMap.find(r->getName());
	if (it == m_resourceHandleMap.end())
	{
		return ResourceHandleStrongPtr();
	}
	return it->second;
}

void ant::ResourceCache::updateLRU( ResourceHandleStrongPtr handle )
{
	m_lruResources.remove(handle);
	m_lruResources.push_front(handle);
}

bool ant::ResourceCache::makeRoom( ant::UInt size )
{
	if (size > m_cacheSize)
	{
		return false;
	}

	// Free one resource at a time until the requirements are met
	while (size > m_cacheSize - m_allocated)
	{
		if (m_lruResources.empty())
		{
			return false;
		}

		freeOneResource();
	}

	return true;
}

char* ant::ResourceCache::allocateMemoery( ant::UInt size )
{
	// Can we make room for new memory?
	if (!makeRoom(size))
	{
		return NULL;
	}

	char* mem = GCC_NEW char[size];
	if (mem)
	{
		m_allocated += size;
	}

	return mem;
}

void ant::ResourceCache::free( ResourceHandleStrongPtr handle )
{
	m_lruResources.remove(handle);
	m_resourceHandleMap.erase(handle->getResource().getName());
	// Note - the resource might still be in use by something,
	// so the cache can't actually count the memory freed until the
	// ResHandle pointing to it is destroyed.

	//m_allocated -= gonner->m_resource.m_size;
	//delete gonner;
}

void ant::ResourceCache::freeOneResource()
{
	ResourceHandleList::iterator it = m_lruResources.end();
	it--;

	ResourceHandleStrongPtr handle = *it;

	m_lruResources.pop_back();
	m_resourceHandleMap.erase(handle->getResource().getName());
}

void ant::ResourceCache::memoryHasBeenFreed( ant::UInt size )
{
	m_allocated -= size;
}

bool ant::ResourceCache::isUsingDevelopmentDirectories( void ) const
{
	GCC_ASSERT(m_file); 
	return m_file->isUsingDevelopmentDirectories(); 
}


