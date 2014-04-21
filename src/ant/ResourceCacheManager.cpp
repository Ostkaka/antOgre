#include <ant/ResourceCacheManager.hpp>
#include <ant/ResourceCache.hpp>

using namespace ant;

ResourceCacheManager* ResourceCacheManager::s_instance = NULL;

void ant::ResourceCacheManager::startUp(void)
{
	if (s_instance)
	{
		ANT_WARNING("Overwriting ResourceCacheManager singleton");
		SAFE_DELETE(s_instance);
	}
	s_instance = ANT_NEW ResourceCacheManager;
}

void ant::ResourceCacheManager::shutDown(void)
{	
	ANT_ASSERT(s_instance);
	SAFE_DELETE(s_instance);
}

ResourceCacheManager* ant::ResourceCacheManager::instance(void)
{
	ANT_ASSERT(s_instance);
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
	ANT_ASSERT(file);
	m_resourceCache = new ResourceCache(sizeInMb,file);

	if (!m_resourceCache->init())
	{
		ANT_ERROR("Failed to initialize resource cache!  Are your paths set up correctly?");
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

