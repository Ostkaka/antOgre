#include <unittests.hpp>
#include <ant/ZipFile.hpp>
#include <ant/ResourceCache.hpp>
#include <ant/Resource.hpp>
#include <ant/ResourceFiles.hpp>
#include <ant/String.hpp>
#include <string.h>

using namespace ant;

namespace 
{

	class Test_ResourceCache : public ::testing::Test
	{

	protected:

		ResourceZipFile * m_file;
		ResourceCache * m_cache;
		std::string zipPath;

		Test_ResourceCache()
		{
			std::cout << "Constructor" << std::endl;
		}

		virtual void SetUp() ANT_OVERRIDE
		{
			std::string zipPath = ANT_DATA_PATH"\\unittestdata\\zipdata.zip";
			m_file = ANT_NEW ResourceZipFile(string2Wstring(zipPath));
			m_cache = ANT_NEW ResourceCache(50,m_file);
		}

		virtual ~Test_ResourceCache() 
		{
			std::cout << "Destructor" << std::endl;
		}

		virtual void TearDown() ANT_OVERRIDE
		{
			std::cout << "tear down" << std::endl;
			m_cache->flush();
			SAFE_DELETE(m_cache);
			SAFE_DELETE(m_file);
		}
	};

	TEST_F(Test_ResourceCache, OpenAndReadFile) 
	{		
		// open the file
		ASSERT_TRUE(m_file->open());
		
		int num = m_file->getNumResources();

		std::cout << "Number of resources: " << num << std::endl;

		for (int i=0 ; i < num ; i++)
		{
			std::cout << "Name: " << m_file->getResourceName(i) << std::endl;
		}
		std::cout << "Ending test OpenAndReadFile" << std::endl;
	}

	TEST_F(Test_ResourceCache, InitCache) 
	{
		// Test init
		ASSERT_TRUE(m_cache->init());
		
		// Try to load all file string
		ant::StringVector vec = m_cache->match("*");

		for (size_t i=0 ; i < vec.size() ; i++)
		{
			std::cout << "Vector: " << vec[i] << std::endl;			
		}

		// Expect it to be 5
		EXPECT_EQ(vec.size(),3);

		// Now, try to load all res-handles into the cache
		for (size_t i = 0 ; i < vec.size(); i++)
		{
			Resource r(vec[i]);
			m_cache->getResourceHandle(&r);
		}

		// Now, clear all the handles and flush the cache
		m_cache->flush();

		ant::UInt uint = m_cache->getAllocatedMemory();
		std::cout << "Allocated size: " << uint << std::endl;

		// Expect the allocated memory to be zero since we flushed the cache
		EXPECT_EQ(uint,0);

		std::cout << "Ending test InitCache" << std::endl;
	}
}
int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}