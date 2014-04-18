#include <ant/resources/XmlResource.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/resources/ResourceCacheManager.hpp>
#include <ant/resources/ResourceCache.hpp>
#include <ant/resources/ResourceHandle.hpp>
#include <ant/resources/IResourceLoader.hpp>
#include <ant/core_types.hpp>

using namespace ant;

void ant::XmlResourceExtraData::parseXml( char* pRawbuffer )
{
	m_xmlDocument.Parse(pRawbuffer);
}

bool ant::XmlResourceLoader::loadResource( char *rawBuffer, ant::UInt rawSize, ResourceHandleStrongPtr handle )
{
	if (rawSize <= 0)
	{
		return false;
	}

	shared_ptr<XmlResourceExtraData> pExtraData = shared_ptr<XmlResourceExtraData>(GCC_NEW XmlResourceExtraData);
	pExtraData->parseXml(rawBuffer);

	handle->setExtraData(shared_ptr<XmlResourceExtraData>(pExtraData));

	return true;
}

TiXmlElement* ant::XmlResourceLoader::loadAndReturnXmlElement( const char* resourceString )
{
	Resource resource(resourceString);
	ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);
	shared_ptr<XmlResourceExtraData> pExtraData = static_pointer_cast<XmlResourceExtraData>(pResourceHandle->getExtraData());
	return pExtraData->getRoot();
}

shared_ptr<IResourceLoader> CreateXmlResourceLoader()
{
	return shared_ptr<IResourceLoader>(GCC_NEW XmlResourceLoader());
}