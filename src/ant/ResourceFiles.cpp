#include <ant/resources/ResourceFiles.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/resources/ZipFile.hpp>
#include <ant/gccUtils/String.hpp>
#include <ant/gccUtils/templates.hpp>
#include <cctype>			// for std::tolower

using namespace ant;

ant::ResourceZipFile::ResourceZipFile( const std::wstring resFileName )
{
	 m_pZipFile = NULL; 
	 m_resFileName=resFileName;
}

ant::ResourceZipFile::~ResourceZipFile()
{
	SAFE_DELETE(m_pZipFile); 
}

bool ant::ResourceZipFile::open()
{
	m_pZipFile = GCC_NEW ZipFile;
	if (m_pZipFile)
	{
		return m_pZipFile->init(m_resFileName.c_str());
	}
	return false;
}

int ant::ResourceZipFile::getRawResourceSize( const Resource& r)
{
	int resourceNum = m_pZipFile->find(r.getName().c_str());
	if (resourceNum == -1)
	{
		return -1;
	}

	return m_pZipFile->getFileLen(resourceNum);
}

int ant::ResourceZipFile::getRawResource( const Resource& r, char *buffer )
{
	int size = 0;
	optional<int> resourceNum = m_pZipFile->find(r.getName().c_str());
	if (resourceNum.valid())
	{
		size = m_pZipFile->getFileLen(*resourceNum);
		m_pZipFile->readFile(*resourceNum,buffer);
	}
	return size;
}

int ant::ResourceZipFile::getNumResources() const
{
	return (m_pZipFile==NULL) ? 0 : m_pZipFile->getNumFiles();
}

std::string ant::ResourceZipFile::getResourceName( int num ) const
{
	std::string retname = "";
	if (m_pZipFile != NULL && num >=0 && num < m_pZipFile->getNumFiles())
	{
		retname = m_pZipFile->getFileName(num);
	}
	return retname;
}

bool ant::ResourceZipFile::isUsingDevelopmentDirectories( void ) const
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
//
//	DevelopmentResourceZipFile
//
//////////////////////////////////////////////////////////////////////////

ant::DevelopmentResourceZipFile::DevelopmentResourceZipFile( const std::wstring resFileName, const Mode mode )
	:ResourceZipFile(resFileName)
{

	m_mode = mode;

	m_AssetsDir = string2Wstring(ANT_DATA_PATH"\\");
}

ant::DevelopmentResourceZipFile::~DevelopmentResourceZipFile()
{

}

int ant::DevelopmentResourceZipFile::find( const std::string &name )
{
	std::string lowerCase = name;
	std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int)) std::tolower);
	ZipContentsMap::const_iterator i = m_DirectoryContentsMap.find(lowerCase);
	if (i==m_DirectoryContentsMap.end())
		return -1;

	return i->second;
}

bool ant::DevelopmentResourceZipFile::open()
{
	if (m_mode != Editor)
	{
		ResourceZipFile::open();
	}

	// open the asset directory and read in the non-hidden contents
	if (m_mode == Editor)
	{
		readAssetsDirectory(L"*");
	}
	else
	{
		// FUTURE WORK - iterate through the ZipFile contents and go grab WIN32_FIND_DATA 
		//   elements for each one. Then it would be possible to compare the dates/times of the
		//   asset in the Zip file with the source asset.
		GCC_ASSERT(0 && "Not implemented yet");
	}

	return true;
}

int ant::DevelopmentResourceZipFile::getRawResourceSize( const Resource& r) 
{
	int size = 0;

	if (m_mode == Editor)
	{
		int num = find(r.getName().c_str());
		if (num == -1)
			return -1;

		return m_AssetFileInfo[num].nFileSizeLow;
	}

	return ResourceZipFile::getRawResourceSize(r);
}

int ant::DevelopmentResourceZipFile::getRawResource( const Resource& r, char *buffer )
{
	if (m_mode == Editor)
	{
		int num = find(r.getName().c_str());
		if (num == -1)
			return -1;

		std::string fullFileSpec = ws2s(m_AssetsDir) + r.getName().c_str(); 
		FILE *f = fopen(fullFileSpec.c_str(), "rb");
		size_t bytes = fread(buffer, 1, m_AssetFileInfo[num].nFileSizeLow, f);
		fclose(f);
		return bytes;
	}

	return ResourceZipFile::getRawResource(r, buffer);
}

int ant::DevelopmentResourceZipFile::getNumResources() const
{
	return (m_mode == Editor) ? m_AssetFileInfo.size() : ResourceZipFile::getNumResources();
}

std::string ant::DevelopmentResourceZipFile::getResourceName( int i ) const
{
	if (m_mode == Editor)
	{
		std::wstring wideName = m_AssetFileInfo[i].cFileName;
		return ws2s(wideName);
	}

	return ResourceZipFile::getResourceName(i);
}

bool ant::DevelopmentResourceZipFile::isUsingDevelopmentDirectories( void )
{
	return true;
}

void ant::DevelopmentResourceZipFile::readAssetsDirectory( std::wstring fileSpec )
{
	HANDLE fileHandle;
	WIN32_FIND_DATA findData;

	// get first file
	std::wstring pathSpec = m_AssetsDir + fileSpec;
	fileHandle = FindFirstFile( pathSpec.c_str(), &findData );
	if( fileHandle != INVALID_HANDLE_VALUE )
	{
		// loop on all remaining entries in dir
		while( FindNextFile( fileHandle,&findData ) )
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				continue;

			std::wstring fileName = findData.cFileName; 
			if( findData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY )
			{
				if (fileName != L".." && fileName != L".")
				{
					fileName = fileSpec.substr(0, fileSpec.length()-1) + fileName + L"\\*";
					readAssetsDirectory(fileName);
				}
			}
			else
			{
				fileName = fileSpec.substr(0, fileSpec.length()-1) + fileName;
				std::wstring lower = fileName;
				std::transform(lower.begin(), lower.end(), lower.begin(), (int(*)(int)) std::tolower);
				wcscpy_s(&findData.cFileName[0], MAX_PATH, lower.c_str());
				m_DirectoryContentsMap[ws2s(lower)] = m_AssetFileInfo.size();
				m_AssetFileInfo.push_back(findData);
			} 
		}
	} 

	FindClose( fileHandle );
}
