#include <unittests.hpp>
#include <antLua/LuaStateManager.hpp>
#include <ant/Events.hpp>
#include <ant/ResourceCacheManager.hpp>
#include <ant/ResourceCache.hpp>
#include <ant/IResourceFile.hpp>
#include <ant/Resource.hpp>
#include <ant/ResourceFiles.hpp>
#include <antLua/LuaScriptExports.hpp>
#include <antLua/ScriptProcess.hpp>
#include <ant/ProcessManagerSingleton.hpp>

using namespace ant;

namespace 
{
	
	const char* SCRIPT_PREINIT_FILE = "lua\\PreInit.lua";

	class Test_LuaStateManager : public ::testing::Test
	{

	protected:

		IResourceFile * m_file;
		shared_ptr<EventManager> TheEventManager;

		Test_LuaStateManager()
		{
			std::cout << "Constructor" << std::endl;
		}

		virtual void SetUp() ANT_OVERRIDE
		{
			// Setup logger
			Logger::Init(ANT_DATA_PATH"\\default_logging.xml");
			m_file = ANT_NEW DevelopmentResourceZipFile(L"Assets.zip", DevelopmentResourceZipFile::Editor);
			TheEventManager.reset(new EventManager("TestEventManager",true));

			// Resource manager
			ResourceCacheManager::startUp();
			ASSERT_TRUE(ResourceCacheManager::instance()->initResourceCache(50,m_file));		
			
			// init the global process manager
			ProcessManagerSingleton::startUp();

			// Init lua stuff
			ASSERT_TRUE(LuaStateManager::startUp());
			LuaScriptExports::registerScripts();
			IScriptProcess::registerScriptClass(); // Should this not be in the register scripts function?

			// Install the script loader
			extern IResourceLoaderStrongPtr CreateScriptResourceLoader();
			ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreateScriptResourceLoader());

			// Start by reading the pre-init file	
			{
				Resource resource(SCRIPT_PREINIT_FILE);
				ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);  
			}

			registerEngineScriptEvents();
		}

		virtual ~Test_LuaStateManager() 
		{
			std::cout << "Destructor" << std::endl;
		}

		virtual void TearDown() ANT_OVERRIDE
		{
			ResourceCacheManager::shutDown();
			LuaStateManager::shutDown();
			LuaScriptExports::unregisterScripts();
			ProcessManagerSingleton::shutDown();
			ScriptEvent::clearAllRegisterdScriptEvents();
			Logger::Destroy();
			std::cout << "Tear down" << std::endl;
		}
	};
}

/**
 * Create listener that will be used to listen to script events
 */
class ScriptEventListener_TestToLua
{
	public:
	
		ScriptEventListener_TestToLua():m_number(0){};
		
		~ScriptEventListener_TestToLua(){};

		void ListenerDelegate(IEventDataStrongPtr pEvent)
		{
			shared_ptr<EvtData_TestFromLua> pCastedEvent = static_pointer_cast<EvtData_TestFromLua>(pEvent);

			m_number = pCastedEvent->getNumber();
		}

		int getNumberFromEvent()
		{
			return m_number;
		}

	protected:
	
	private:
		int m_number;
};

TEST_F(Test_LuaStateManager, ScriptEventListeners) 
{
	// Now, load the test file with the sample test script
	{
		Resource resource("lua\\testScript.lua");
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);  
	}

	// Create listener that will recieve and event from lua
	ScriptEventListener_TestToLua listener;
	EventListenerDelegate delegateFunc = MakeDelegate(&listener,&ScriptEventListener_TestToLua::ListenerDelegate);
	IEventManager::instance()->addListener(delegateFunc,EvtData_TestFromLua::sk_EventType);
	 // Now, create a test event that is sent to Lua and then sent back to C++
	shared_ptr<EvtData_TestToLua> pEvent(ANT_NEW EvtData_TestToLua);
	IEventManager::instance()->queueEvent(pEvent);

	// Update the processes a step. Two steps for the message to propagate all the way!
	IEventManager::instance()->update();
	IEventManager::instance()->update();

	// This should have been handled one time. So the number should be 1
	ASSERT_EQ(listener.getNumberFromEvent(),1);

}

TEST_F(Test_LuaStateManager, Init) 
{

	// Now, try to find the init functions in the lua state
	LuaPlus::LuaObject obj = LuaStateManager::instance()->getGlobalVars().GetByName("class");

	// The class function should have been imported by now
	ASSERT_TRUE(obj.IsFunction());

	// Se that the function for exporting eventlisteners are correctly exported
	// Now, try to find the init functions in the lua state
	LuaPlus::LuaObject ev = LuaStateManager::instance()->getGlobalVars().GetByName("registerEventListener");

	// The class function should have been imported by now
	ASSERT_TRUE(ev.IsFunction());

	// Now, load the test file with the sample test script
	{
		Resource resource("lua\\testScript.lua");
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);  
	}

	// Check so that the TestProcess is loaded
	LuaPlus::LuaObject metaTableObj = LuaStateManager::instance()->getGlobalVars().Lookup("ScriptProcess");
	ASSERT_TRUE(!metaTableObj.IsNil());
}

TEST_F(Test_LuaStateManager, ScriptProcess) 
{

	// Now, load the test file with the sample test script
	{
		Resource resource("lua\\testScript.lua");
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);  
	}

	// Test script process
	LuaPlus::LuaObject obj = LuaStateManager::instance()->getGlobalVars().GetByName("TestProcess");

	// See so that the function exists
	ASSERT_TRUE(obj.IsFunction());

	// Do the test function and insert a process in the global process manager
	LuaPlus::LuaFunction<void> func(obj);
	func(LuaStateManager::instance()->getLuaState());

	// We should now have one process in the manager
	ASSERT_TRUE((ProcessManagerSingleton::instance()->getProcessManager()->getNumProcesses() == 1));

	// loop for a while
	ant::DeltaTime dt = 1;
	ant::DeltaTime currT = 0;	
	ant::DeltaTime endT= 5;
	while(currT < endT)
	{
		ProcessManagerSingleton::instance()->getProcessManager()->updateProcesses(dt);
		currT += dt;
	}
	
	// Check so that the process object exists	
	LuaPlus::LuaObject sProcess = LuaStateManager::instance()->getGlobalVars().GetByName("sParent");
	ASSERT_TRUE(!sProcess.IsNil());
	ASSERT_TRUE(sProcess.IsTable());

	//Check that finished is set to true
	LuaPlus::LuaObject finished = sProcess.GetByName("finished");
	ASSERT_TRUE(finished.IsBoolean());

	// It should be true since we looped after the execute
	ASSERT_TRUE(finished.GetBoolean());

	// Now, loop a little more 
	endT = 10;
	dt = 0.1;
	while(currT < endT)
	{
		ProcessManagerSingleton::instance()->getProcessManager()->updateProcesses(dt);
		currT += dt;
	}

	// Get the process child
	LuaPlus::LuaObject cProcess = LuaStateManager::instance()->getGlobalVars().GetByName("sChild");
	ASSERT_TRUE(!cProcess.IsNil());
	ASSERT_TRUE(cProcess.IsTable());

	//Check that finished is set to true
	LuaPlus::LuaObject cfinished = cProcess.GetByName("finished");
	ASSERT_TRUE(cfinished.IsBoolean());

	// It should be true since we looped after the execute
	ASSERT_TRUE(cfinished.GetBoolean());
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}