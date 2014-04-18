#include <ant/resources/ResourceCacheManager.hpp>
#include <ant/resources/ResourceCache.hpp>

using namespace ant;

ResourceCacheManager* ResourceCacheManager::s_instance = NULL;

void ant::ResourceCacheManager::create(void)
{
	if (s_instance)
	{
		GCC_WARNING("Overwriting ResourceCacheManager singleton");
		SAFE_DELETE(s_instance);
	}
	s_instance = GCC_NEW ResourceCacheManager;
}

void ant::ResourceCacheManager::destroy(void)
{	
	GCC_ASSERT(s_instance);
	SAFE_DELETE(s_instance);
}

ResourceCacheManager* ant::ResourceCacheManager::instance(void)
{
	GCC_ASSERT(s_instance);
	return s_instance;
}

ant::ResourceCacheManager::ResourceCacheManager(void)
{
	m_resourceCache = NULL;
}

ant::ResourceCacheManager::~ResourceCacheManager(void)
{
	clearCaches();
}

bool ant::ResourceCacheManager::initResourceCache( const unsigned int sizeInMb, IResourceFile *file )
{
	GCC_ASSERT(file);
	m_resourceCache = new ResourceCache(sizeInMb,file);

	if (!m_resourceCache->init())
	{
		GCC_ERROR("Failed to initialize resource cache!  Are your paths set up correctly?");
		return false;
	}

	return true;
}

void ant::ResourceCacheManager::clearCaches(void)
{
	if (m_resourceCache)
	{
		SAFE_DELETE(m_resourceCache);
	}
}

