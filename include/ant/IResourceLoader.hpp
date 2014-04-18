#ifndef IRESOURCELOADER_HPP_
	#define IRESOURCELOADER_HPP_

#include <ant/core_types.hpp>

namespace ant
{

	/**
	 * Class that loads resources of a specific types
	 */
	class IResourceLoader
	{
	public:
		/// Default constructor
		IResourceLoader(){};

		/// Default destructor
		virtual ~IResourceLoader(){};

		/** 
		* Get the Wildcard pattern that the resource cache uses for distinguish which loaders 
		* to use for which files
		*/
		virtual std::string getPattern()=0;

		/// Returns true if the bits in the raw file can be used without processing
		virtual bool useRawFile()=0;

		/// I have no idea
		virtual bool discardRawBufferAfterLoad()=0;

		/// I have no idea
		virtual bool addNULLZero(){return false;}

		/// Method that defines the size of the resource if ti is different from the size stored in the file
		virtual ant::UInt getLoadedResourceSize(char* rawBuffer, ant::UInt rawSize)=0;

		/// Loads the resource into a raw buffer
		virtual bool loadResource(char* rawBuffer, ant::UInt rawSize, ResourceHandleStrongPtr handle)=0;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////

	protected:
			
	private:

	};

}

#endif