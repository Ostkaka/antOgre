#include <unittests.hpp>
#include <antLua/LuaStateManager.hpp>
#include <ant/Events.hpp>
#include <ant/EventManager.hpp>
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
#include <ant/BaseGameLogic.hpp>

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

			// Init event manager
			EventManager* mgr = ANT_NEW EventManager("GlobalEventManager", true);
			
			// Init lua stuff
			ASSERT_TRUE(LuaStateManager::startUp());
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

			// Create game logic
			m_logic = new BaseGameLogic();

			// Init the logic
			ASSERT_TRUE(m_logic->init());

			std::cout << "Set up" << std::endl;
		}

		virtual void TearDown() ANT_OVERRIDE
		{
			BaseScriptComponent::unregisterScriptFunctions();
			//LuaStateManager::destroy();
			//ResourceCacheManager::destroy();
			LuaScriptExports::unregisterScripts();
			ScriptEvent::clearAllRegisterdScriptEvents();
			SAFE_DELETE(m_logic);
			SAFE_DELETE(m_file);
			Logger::Destroy();
			std::cout << "Tear down" << std::endl;
		}

		virtual ~Test_Actor() 
		{
			std::cout << "Destructor" << std::endl;
		}

	protected:
		IResourceFile* m_file;
		BaseGameLogic* m_logic;
	};
}

/**
 * This test handles the construction and manipulation of actors
 */
TEST_F(Test_Actor, initBaseGameLogic)
{
	// Create an actor 
	ActorId id;
	{
		ActorStrongPtr pActor = m_logic->createActor("actors\\testActor.xml");
		id = pActor->getId();
	}
	
	// Try to update the logic
	ant::DeltaTime dt = 0.1;
	ant::DeltaTime time = 0.0;
	ant::DeltaTime endTime = 1.0;
	while( time < endTime )
	{
		m_logic->onUpdate(time,dt);
		time+=dt;
	}

	// Now, try to remove the actor from the logic
	m_logic->destroyActor(id);

	// Now, try to get the actor from the id. This should not happen
	ASSERT_TRUE(m_logic->getActor(id).expired());

}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}