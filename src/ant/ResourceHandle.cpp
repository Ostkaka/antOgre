#include <ant/resources/ResourceHandle.hpp>
#include <ant/resources/ResourceCache.hpp>

using namespace ant;

ant::ResourceHandle::ResourceHandle( Resource &resource, char* buffer, ant::UInt size, ResourceCache * pResCache )
	:m_resource(resource)
{
	m_buffer = buffer;
	m_size = size;
	m_extra = NULL;
	m_pResCache = pResCache;
}

ant::ResourceHandle::~ResourceHandle()
{
	SAFE_DELETE_ARRAY(m_buffer);
	m_pResCache->memoryHasBeenFreed(m_size);
}





