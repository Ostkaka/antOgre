#ifndef RESOURCELOADERS_HPP_
	#define RESOURCELOADERS_HPP_

#include <ant/core_types.hpp>
#include <ant/resources/IResourceLoader.hpp>

namespace ant
{

	/**
	 * Default resource loader the loads files from a resource file as-is
	 */
	class DefaultResourceLoader : public IResourceLoader
	{
	public:
	
		/// Constructor
		DefaultResourceLoader();

		/// Destructor
		virtual ~DefaultResourceLoader();

		/** 
		* Get the Wildcard pattern that the resource cache uses for distinguish which loaders 
		* to use for which files
		*/
		virtual std::string getPattern();

		/// Returns true if the bits in the raw file can be used without processing
		virtual bool useRawFile();

		/// Returns true if the buffer should be discarded after loading it from disk
		virtual bool discardRawBufferAfterLoad();

		/// Method that defines the size of the resource if ti is different from the size stored in the file
		virtual ant::UInt getLoadedResourceSize(char* rawBuffer, ant::UInt rawSize);

		/// Loads the resource into a raw buffer
		virtual bool loadResource(char* rawBuffer, ant::UInt rawSize, ResourceHandleStrongPtr handle);

	};

	/**
	 * This resource loader handles loading of script files
	 */
	class ScriptResourceLoader : public IResourceLoader
	{
	public:
		virtual bool useRawFile() { return false; }
		virtual bool discardRawBufferAfterLoad() { return true; }
		virtual bool addNULLZero() { return true; }
		virtual ant::UInt getLoadedResourceSize(char* rawBuffer, ant::UInt rawSize) { return rawSize; }
		virtual bool loadResource(char *rawBuffer, unsigned int rawSize, ResourceHandleStrongPtr handle);
		virtual std::string getPattern() { return "*.lua"; }
	};

}

#endif