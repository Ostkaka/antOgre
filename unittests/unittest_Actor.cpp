#include <unittests.hpp>
#include <antLua/LuaStateManager.hpp>
#include <ant/Events.hpp>
#include <ant/ResourceCacheManager.hpp>
#include <ant/ResourceCache.hpp>
#include <ant/IResourceFile.hpp>
#include <actors/ActorFactory.hpp>
#include <actors/Actor.hpp>
#include <ant/XmlResource.hpp>
#include <antLua/BaseScriptComponent.hpp>
#include <ant/Resource.hpp>
#include <ant/ResourceFiles.hpp>
#include <antLua/LuaScriptExports.hpp>
#include <antLua/ScriptProcess.hpp>
#include <ant/ProcessManagerSingleton.hpp>

using namespace ant;

namespace 
{
	const char* SCRIPT_PREINIT_FILE = "lua\\PreInit.lua";

	class Test_Actor : public ::testing::Test
	{
	protected:

		Test_Actor()
		{
			std::cout << "Constructor" << std::endl;
		}

		virtual void SetUp() ANT_OVERRIDE
		{
			// Setup logger
			Logger::Init(ANT_DATA_PATH"\\default_logging.xml");
			m_file = ANT_NEW DevelopmentResourceZipFile(L"Assets.zip", DevelopmentResourceZipFile::Editor);

			// Resource manager
			ResourceCacheManager::startUp();
			ASSERT_TRUE(ResourceCacheManager::instance()->initResourceCache(50,m_file));	

			// init the global process manager
			ProcessManagerSingleton::startUp();

			// Init lua stuff
			ASSERT_TRUE(LuaStateManager::create());
			LuaScriptExports::registerScripts();
			IScriptProcess::registerScriptClass(); // Should this not be in the register scripts function?
			BaseScriptComponent::registerScriptFunctions();

			// Install the script loader
			extern IResourceLoaderStrongPtr CreateScriptResourceLoader();
			extern IResourceLoaderStrongPtr CreateXmlResourceLoader();
			ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreateScriptResourceLoader());
			ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreateXmlResourceLoader());

			// Start by reading the pre-init file	and another stuff
			{
				Resource resource(SCRIPT_PREINIT_FILE);
				ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);  

				Resource resource2("lua\\PreInit.lua");
				ResourceHandleStrongPtr pResourceHandle2 = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource2);  

				Resource resource3("lua\\Levelinit.lua");
				ResourceHandleStrongPtr pResourceHandle3 = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource3);  
			}

			registerEngineScriptEvents();

			mActorFactory.reset(ANT_NEW ActorFactory());

			std::cout << "Set up" << std::endl;
		}

		virtual void TearDown() ANT_OVERRIDE
		{
			BaseScriptComponent::unregisterScriptFunctions();
			//LuaStateManager::destroy();
			//ResourceCacheManager::destroy();
			LuaScriptExports::unregisterScripts();
			ProcessManagerSingleton::shutDown();
			ScriptEvent::clearAllRegisterdScriptEvents();
			Logger::Destroy();
			std::cout << "Tear down" << std::endl;
		}

		virtual ~Test_Actor() 
		{
			std::cout << "Destructor" << std::endl;
		}

	protected:
		IResourceFile * m_file;
		shared_ptr<ActorFactory> mActorFactory;
	};
}

/**
 * This test handles the construction and manipulation of actors
 */
TEST_F(Test_Actor, initActors)
{
	// Try to load actor .xml.
	ActorId id = 1;
	ActorStrongPtr pActor = mActorFactory->createActor("actors\\testActor.xml",NULL,id);

	// Verify
	ASSERT_TRUE(pActor);

	// Verify type
	ASSERT_EQ(pActor->getType(),"test");

	// Verify number of components
	ASSERT_EQ(pActor->getComponents()->size(),1);

	ActorComponentWeakPtr comp = pActor->getComponent<BaseScriptComponent>(BaseScriptComponent::g_Name);

	// Now, destroy the component and check 
	pActor->destroy();
	ASSERT_EQ(pActor->getComponents()->size(),0);
	
	// Weak pointer should be expired
	ASSERT_TRUE(comp.expired());

	// Reset the actor pointer
	pActor.reset();
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}