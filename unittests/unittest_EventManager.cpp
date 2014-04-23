#include <unittests.hpp>
#include <ant/EventManager.hpp>
#include <ant/Events.hpp>
#include <ant/core_types.hpp>
#include <string.h>

using namespace ant;

class Test_EventManager : public ::testing::Test
{
protected:

	Test_EventManager()
	{
		std::cout << "Constructor" << std::endl;
	}

	virtual void SetUp() ANT_OVERRIDE
	{
		// Setup logger
		Logger::Init("Test_EventManager.log");

		TheEventManager.reset(ANT_NEW EventManager("TestEventManager",false));
		std::cout << "Set up" << std::endl;
	}

	virtual void TearDown() ANT_OVERRIDE
	{
	}

	virtual ~Test_EventManager() 
	{
		std::cout << "Destructor" << std::endl;
	}

protected:
	shared_ptr<EventManager> TheEventManager;
};

class SimpleTester_AddRemove
{
public:
	SimpleTester_AddRemove()
	{
	}
	
	~SimpleTester_AddRemove()
	{
	}

	bool addListener(EventManager * manager)
	{
		ANT_ASSERT(manager);
		if (manager)
		{
			// Create and insert the delegate
			EventListenerDelegate delegateFunc = MakeDelegate(this,&SimpleTester_AddRemove::execute_TestEvent);
			this->m_manager = manager;
			return manager->addListener(delegateFunc,EvtData_TestExecute::sk_EventType);
		}
		return false;
	}

	bool removeListener(EventManager * manager)
	{
		ANT_ASSERT(manager);
		if (manager)
		{
			// Create and insert the delegate
			EventListenerDelegate delegateFunc = MakeDelegate(this,&SimpleTester_AddRemove::execute_TestEvent);
			this->m_manager = manager;
			return manager->removeListener(delegateFunc,EvtData_TestExecute::sk_EventType);
		}
		return false;
	}

	void execute_TestEvent(IEventDataStrongPtr pEvent)
	{
		shared_ptr<EvtData_TestExecute> pCastedEvent = static_pointer_cast<EvtData_TestExecute>(pEvent);

		pCastedEvent->execute();
	}

	EventManager * m_manager;
};

class SimpleTester_Execute
{
public:
	SimpleTester_Execute(){}
	~SimpleTester_Execute(){}

	void init(EventManager * manager)
	{
		ANT_ASSERT(manager);
		if (manager)
		{
			// Create and insert the delegate
			EventListenerDelegate delegateFunc = MakeDelegate(this,&SimpleTester_Execute::execute_TestEvent);
			manager->addListener(delegateFunc,EvtData_TestExecute::sk_EventType);
		}
	}

	void execute_TestEvent(IEventDataStrongPtr pEvent)
	{
		shared_ptr<EvtData_TestExecute> pCastedEvent = static_pointer_cast<EvtData_TestExecute>(pEvent);

		pCastedEvent->execute();
	}
};

TEST_F(Test_EventManager, TestSimpleEvent)
{	
	// Create a tester and insert the manager
	SimpleTester_Execute tester;
	tester.init(TheEventManager.get());

	// Create event and send it to the manager
	IEventDataStrongPtr ev(ANT_NEW EvtData_TestExecute());

	TheEventManager->triggerEvent(ev);

	// Event should have been executed
	EXPECT_TRUE(((EvtData_TestExecute*)(ev.get()))->isExecuted());
}

TEST_F(Test_EventManager, TestAddRemove)
{	
	// Create a tester and insert the manager
	SimpleTester_AddRemove * tester = new SimpleTester_AddRemove();
	
	// Test add/remove
	EXPECT_TRUE(tester->addListener(TheEventManager.get()));
	EXPECT_TRUE(tester->removeListener(TheEventManager.get()));
	
	// Send an event to manager, should not be executed
	IEventDataStrongPtr ev(ANT_NEW EvtData_TestExecute());
	TheEventManager->triggerEvent(ev);
	EXPECT_FALSE(((EvtData_TestExecute*)(ev.get()))->isExecuted());

	SAFE_DELETE(tester);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}