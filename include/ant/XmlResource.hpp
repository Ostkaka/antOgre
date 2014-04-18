#ifndef XMLRESOURCE_HPP_
	#define XMLRESOURCE_HPP_

#include <ant/core_types.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/resources/IResourceLoader.hpp>
#include <tinyxml.h>

namespace ant
{

	/**
	 * The extra data that couples to the resource of XML type.
	 */
	class XmlResourceExtraData : public IResourceExtraData
	{
	public:

		virtual std::string toString();
		void parseXml(char* pRawbuffer);
		TiXmlElement* getRoot(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		TiXmlDocument m_xmlDocument;	

	};

	/// Implementation
	ANT_INLINE std::string XmlResourceExtraData::toString() { return "XmlResourceExtraData"; }
	ANT_INLINE TiXmlElement* XmlResourceExtraData::getRoot() { return m_xmlDocument.RootElement(); }

	/**
	 * Loader for XML type resources
	 */
	class XmlResourceLoader : public IResourceLoader
	{
	public:
		
		virtual bool useRawFile();
		virtual bool discardRawBufferAfterLoad();
		virtual ant::UInt getLoadedResourceSize(char *rawBuffer, ant::UInt rawSize);
		virtual bool loadResource(char *rawBuffer, ant::UInt rawSize, ResourceHandleStrongPtr handle);
		virtual std::string getPattern();

		/// Static helper function
		static TiXmlElement* loadAndReturnXmlElement(const char* resourceString);

	protected:
		
	private:
	};

	/// Implementation
	ANT_INLINE std::string XmlResourceLoader::getPattern() { return "*.xml"; }
	ANT_INLINE bool XmlResourceLoader::useRawFile() { return false; }
	ANT_INLINE bool XmlResourceLoader::discardRawBufferAfterLoad() { return false; }
	ANT_INLINE ant::UInt XmlResourceLoader::getLoadedResourceSize(char *rawBuffer, ant::UInt rawSize) { return rawSize; }
}

#endif