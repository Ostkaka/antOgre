#ifndef TEXTURERESOURCE_HPP_
	#define TEXTURERESOURCE_HPP_

#include <ant/core_types.hpp>
#include <ant/resources/IResourceLoader.hpp>
#include <ant/resources/Resource.hpp>

namespace ant
{
	/**
	 * Resource loader for loading PNG files 
	 */
	class SFMLTextureResourceLoader : public IResourceLoader
	{
		virtual bool useRawFile() ANT_OVERRIDE { return true; }
		virtual bool discardRawBufferAfterLoad() ANT_OVERRIDE { return false; }
		virtual ant::UInt getLoadedResourceSize(char* rawBuffer, ant::UInt rawSize) ANT_OVERRIDE;
		virtual bool loadResource(char *rawBuffer, unsigned int rawSize, ResourceHandleStrongPtr handle) ANT_OVERRIDE;
	};

	/**
	 * Loader for PNG files
	 */
	class PngResourceLoader : public SFMLTextureResourceLoader
	{
		virtual std::string getPattern() { return "*.png"; }
	};

	/**
	* Loader for JPG files
	*/
	class JpgResourceLoader : public SFMLTextureResourceLoader
	{
		virtual std::string getPattern() { return "*.jpg"; }
	};
}

#endif