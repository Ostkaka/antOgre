#ifndef RESOURCECACHEMANAGER_HPP_
	#define RESOURCECACHEMANAGER_HPP_

#include <ant/core_types.hpp>
#include <ant/IResourceFile.hpp>
#include <ant/ResourceCache.hpp>

namespace ant
{
	/**
	 * Singleton class that handles the management of resource cache
	 * This will act as sample manager for handling resource caches now. Not a long term solution.
	 */
	class ResourceCacheManager : Singleton
	{

	public:

		/// Creates an instance of the ResourceCacheManager
		static void startUp(void);

		/// Destroys the instance and all resource caches inside
		static void shutDown(void);

		/// Get instance
		static ResourceCacheManager* instance(void);

		/// Initialized the resource cache in the manager
		bool initResourceCache( const unsigned int sizeInMb, IResourceFile *file );

		/// Returns the cache used by the manager
		ResourceCache* getResourceCache(void);

	private:

		// Clear all resource caches
		void clearCaches(void);

		// Make these private due to Singleton
		ResourceCacheManager(void);
		virtual ~ResourceCacheManager(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:

		ResourceCache *m_resourceCache;

		static ResourceCacheManager* s_instance;
	};

	/*Implementation*/
	ANT_INLINE ResourceCache* ResourceCacheManager::getResourceCache() 
	{ 		
		return m_resourceCache; 
	}
}
#endif