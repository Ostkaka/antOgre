#include <unittests.hpp>
#include <ant/ZipFile.hpp>
#include <string.h>

using namespace ant;

void makePath(const char *pszPath)
{
	if (pszPath[0] == '\0')
		return;

	char buf[1000];
	const char *p = pszPath;

	//  printf("MakePath(\"%s\")\n", pszPath);

	// Skip machine name in network paths like \\MyMachine\blah...
	if (p[0] == '\\' && p[1] == '\\')
		p = strchr(p+2, '\\');

	while (p != NULL && *p != '\0')
	{
		p = strchr(p, '\\');

		if (p)
		{
			memcpy(buf, pszPath, p - pszPath);
			buf[p - pszPath] = 0;
			p++;
		}
		else
			strcpy(buf, pszPath);

		if (buf[0] != '\0' && strcmp(buf, ".") && strcmp(buf, ".."))
		{
			//      printf("  Making path: \"%s\"\n", buf);
			mkdir(buf);
		}
	}
}

TEST(Test_Resource, ZipFile_test)
{	
	// Try to load a zipfile
	std::string zippath = ANT_DATA_PATH"\\unittestdata\\zipdata.zip";
	std::wstring wzippath = string2Wstring(zippath);
	
	// Try to open the zipfile
	FILE * f = fopen(zippath.c_str(),"rb");

	//Check if fopen is successful
	ASSERT_TRUE(f != NULL);

	if (f)
	{
		ZipFile zip;
	
		// Did we open it OK?
		ASSERT_TRUE(zip.init(wzippath));
		
		{
			for (int i = 0; i < zip.getNumFiles(); i++)
			{
				int len = zip.getFileLen(i);

				std::string fname =  zip.getFileName(i);

				printf("File \"%s\" (%d bytes): ", fname, len);

				char *pData = ANT_NEW char[len];
				if (!pData)
					printf("OUT OF MEMORY\n");
				else if (true == zip.readFile(i, pData))
				{
					printf("OK\n");
					char dpath[1000];

					sprintf(dpath, "Data\\%s", fname);
					char *p = strrchr(dpath, '\\');
					if (p)
					{
						*p = '\0';
						makePath(dpath);
						*p = '\\';
					}
					FILE *fo = fopen(dpath, "wb");
					if (fo)
					{
						fwrite(pData, len, 1, fo);
						fclose(fo);
					}
				}
				else
					printf("ERROR\n");
				delete[] pData;
			}
			zip.end();
		}
		fclose(f);
	}
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}