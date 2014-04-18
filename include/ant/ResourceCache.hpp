#ifndef RESOURCECACHE_HPP_
	#define RESOURCECACHE_HPP_

#include <ant/core_types.hpp>

namespace ant
{

	typedef std::list< ResourceHandleStrongPtr > ResourceHandleList;
	typedef std::map< std::string , ResourceHandleStrongPtr > ResourceHandleMap;
	typedef std::list< IResourceLoaderStrongPtr > ResourceLoaders;

	/**
	 * Class that manages resources 
	 */
	class ResourceCache
	{
		friend class ResourceHandle;

	public:

		/// Default constructor
		ResourceCache(const unsigned int sizeInMb, IResourceFile *file);
	
		/// Default destructor
		virtual ~ResourceCache();

		/// Inits the resource cache
		bool init();

		/// Register a loader type with the resource cache
		void registerLoader(IResourceLoaderStrongPtr loader);

		/// Gets a resource handle from a resource identifier
		ResourceHandleStrongPtr getResourceHandle(Resource *r);

		/// Pre-loads resources with a given pattern into the cache
		int preLoad(const std::string pattern, void (*progressCallback)(int,bool &));

		/// Returns a vector with string identifiers that match the extension patten given
		std::vector<std::string> match(const std::string pattern);

		/// Get the current allocated memory of the cache
		ant::UInt getAllocatedMemory() const;

		/// Flush the entire cache of all loaded resources
		void flush();

		/// Returns true if cache uses developer directories
		bool isUsingDevelopmentDirectories(void) const;

	protected:

		/// Tries to find a resource handle form a resource specifier
		ResourceHandleStrongPtr find(Resource *F);

		/// Loads a resource handle form a resource specifier
		ResourceHandleStrongPtr load(Resource *r);

		/// Update lru by placing the specified handle in the front
		void updateLRU(ResourceHandleStrongPtr handle);

		/// Clears and makes room in the cache
		bool makeRoom(ant::UInt size);

		/// Try to allocate memory from the cache
		char* allocateMemoery(ant::UInt size);

		/// Free a loaded resource from the cache
		void free(ResourceHandleStrongPtr handle);

		/// Remove the least used resource from the cache
		void freeOneResource();

		/// Signal the memory has been freed
		void memoryHasBeenFreed(ant::UInt size);
	
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		/// Least recently used list
		ResourceHandleList m_lruResources;

		/// Map holding the total loaded resources of the cache
		ResourceHandleMap m_resourceHandleMap;

		/// Map holding the resource loaders of the cache
		ResourceLoaders m_resourceLoaderMap;

		/// The resource file that the cache is connected to
		IResourceFile * m_file;

		/// Cache size in bytes
		ant::UInt m_cacheSize;

		/// Currently allocated size in bytes
		ant::UInt m_allocated;
	};

	/**
	 *  Implementation
	 */
	ANT_INLINE ant::UInt ResourceCache::getAllocatedMemory() const {return m_allocated;}

}

#endif