#ifndef IRESOURCEFILE_HPP_
	#define IRESOURCEFILE_HPP_

#include <ant/core_types.hpp>

namespace ant
{
	/**
	* Interface class for declaring resource files
	*/
	class IResourceFile
	{
	public:

		/// Default constructor
		IResourceFile(){};
		
		/// Default destructor
		virtual ~IResourceFile(){};

		/// Tries to open the resource file
		virtual bool open()=0;

		/// Get the raw resource data size
		virtual int getRawResourceSize(const Resource&)=0;

		/// Get the raw resource of the handle
		virtual int getRawResource(const Resource& r, char *buffer)=0;

		/// Get the number of resources from the file
		virtual int getNumResources() const = 0;

		/// Returns the name of the resource in the file given an index
		virtual std::string getResourceName(int i) const = 0;

		/// Returns true if file is using developer directories
		virtual bool isUsingDevelopmentDirectories(void) const = 0;
	};
}

#endif

