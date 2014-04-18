#ifndef RESOURCEFILES_HPP_
	#define RESOURCEFILES_HPP_

#include <ant/core_types.hpp>
#include <ant/resources/IResourceFile.hpp>
#include <ant/resources/ZipFile.hpp>

namespace ant
{

	class ZipFile;

	/**
	 * Class that implements IResourceFile for use with a zip file
	 */
	class ResourceZipFile : public ant::IResourceFile
	{

	public:

		/// Default constructor
		ResourceZipFile(const std::wstring resFileName);
	
		/// Default destructor
		virtual ~ResourceZipFile();

		/// Tries to open the resource file
		virtual bool open();

		/// Get the raw resource data size
		virtual int getRawResourceSize(const Resource&);

		/// Get the raw resource of the handle
		virtual int getRawResource(const Resource& r, char *buffer);

		/// Get the number of resources from the file
		virtual int getNumResources() const;

		/// Returns the name of the resource in the file given an index
		virtual std::string getResourceName(int num) const;

		/// Returns true if file is using developer directories
		virtual bool isUsingDevelopmentDirectories(void) const;


		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		/// The resource file this calss connects to
		ZipFile* m_pZipFile;
		/// File path of the file
		std::wstring m_resFileName;
	};


	/**
	 * Resourcefile that handles folders that are non-crompressed, which is easier during development
	 */
	class DevelopmentResourceZipFile : public ResourceZipFile
	{
	public:

		enum Mode
		{
			Development,
			Editor
		};

		DevelopmentResourceZipFile(const std::wstring resFileName, const Mode mode);

		virtual ~DevelopmentResourceZipFile();

		/// Tries to open the resource file
		virtual bool open();

		/// Get the raw resource data size
		virtual int getRawResourceSize(const Resource&);

		/// Get the raw resource of the handle
		virtual int getRawResource(const Resource& r, char *buffer);

		/// Get the number of resources from the file
		virtual int getNumResources() const;

		/// Returns the name of the resource in the file given an index
		virtual std::string getResourceName(int i) const;

		/// Returns true if file is using developer directories
		virtual bool isUsingDevelopmentDirectories(void);

		/// finds something
		int find(const std::string &path);

	protected:

			void readAssetsDirectory(std::wstring fileSpec);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:

		Mode m_mode;

		std::wstring m_AssetsDir;

		std::vector<WIN32_FIND_DATA> m_AssetFileInfo;

		ZipContentsMap m_DirectoryContentsMap;

	};
}

#endif