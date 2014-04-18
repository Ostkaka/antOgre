#ifndef ZIP_FILE_HPP_
	#define ZIP_FILE_HPP_

#include <stdio.h>
#include <map>

namespace ant 
{

	typedef std::map<std::string,int> ZipContentsMap; 

	/*
	* Utility file for loading and handling zip files
	*/
	class ZipFile
	{
	public:
		/// Default constructor
		ZipFile();

		/// Default destructor
		virtual ~ZipFile();

		bool init(const std::wstring &resfileName);

		void end();

		int getNumFiles() const;

		std::string getFileName(int id) const;

		int getFileLen(int id) const;

		bool readFile(int id, void* pBuf);

		// Can be used for multi thread decompression
		bool readLargeFile(int id, void* pBuf, void (*progressCallback)(int, bool &));

		int find(const std::string &path) const;

		ZipContentsMap m_ZipContentsMap;

	protected:
	
	private:

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	private:

		struct TZipDirHeader;
		struct TZipDirFileHeader;
		struct TZipLocalHeader;

		/// The zip file
		FILE * m_pFile;
		/// Raw data
		char * m_pDirData;
		/// number of entries
		int m_numEntries;
		/// Pointer to dir entries in the pDirData
		const TZipDirFileHeader **m_papDir;

	};
}
#endif