#ifndef RESOURCEHANDLE_HPP_
	#define RESOURCEHANDLE_HPP_

#include <ant/core_types.hpp>
#include <ant/resources/Resource.hpp>

// Forward declaration
namespace ant
{
	class ResourceCache;
	class IResourceExtraData;
}

namespace ant
{

	typedef shared_ptr<IResourceExtraData> ResourceExtraDataStrongPtr;

	/**
	 * Class that couples loaded resource data with an resource identifier Resource
	 */
	class ResourceHandle
	{
		friend class ResourceCache;
	
	public:

		/// Default constructor
		ResourceHandle(Resource &resource, char* buffer, ant::UInt size, ResourceCache * pResCache);

		/// Default destructor
		virtual ~ResourceHandle();

		/// Returns the raw size of the resource
		ant::UInt getSize() const; // inline this shit

		/// Returns the raw buffer data of the resource
		char* getBuffer() const; //inline

		/// Returns a buffer pointer that can be written to
		char* getWritablebuffer(); //inline

		/// Returns the extra data associated with this object
		ResourceExtraDataStrongPtr getExtraData(); //inline

		/// Sets the extra data of this resource
		void setExtraData(ResourceExtraDataStrongPtr extra); //inline

	protected:

		/// Get the resource identifier for the handle
		const Resource& getResource();

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		/// Resource identifier
		Resource m_resource;
		/// Raw resource data
		char* m_buffer;
		/// size if the data
		ant::UInt m_size;
		/// Pointer to the extra data
		ResourceExtraDataStrongPtr m_extra;
		/// Raw pointer to the cache this 
		ResourceCache *m_pResCache;
		
	};

	/*Implementation*/

	ANT_INLINE ant::UInt ant::ResourceHandle::getSize() const
	{
		return m_size;
	}

	ANT_INLINE char* ant::ResourceHandle::getBuffer() const
	{
		return m_buffer;
	}

	ANT_INLINE char* ant::ResourceHandle::getWritablebuffer()
	{
		return m_buffer;
	}

	ANT_INLINE ant::ResourceExtraDataStrongPtr ant::ResourceHandle::getExtraData()
	{
		return m_extra;
	}

	ANT_INLINE void ant::ResourceHandle::setExtraData( ResourceExtraDataStrongPtr extra )
	{
		m_extra = extra;
	}

	ANT_INLINE const ant::Resource& ant::ResourceHandle::getResource()
	{
		return m_resource;
	}
}
#endif