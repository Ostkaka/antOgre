#include <unittests.hpp>
#include <ant/classes/ProcessManager.hpp>
#include <ant/interfaces/IProcess.hpp>

using namespace ant;

class DelayProcess : public IProcess
{
public:

	explicit DelayProcess (ant::UInt timeToDelay)
	{
		m_timeToDelay = timeToDelay;
		m_timeDelayedSoFar = 0;
	}

	~DelayProcess (){};

protected:
	virtual void onUpdate(ant::DeltaTime dt){
		m_timeDelayedSoFar += dt;

		if (m_timeDelayedSoFar > m_timeToDelay)
		{
			std::cout << "Delay stopped!" << std::endl;
			succeedAndStop();
		}
	}

protected:
	ant::DeltaTime m_timeToDelay;
	ant::DeltaTime m_timeDelayedSoFar;

};

class TagProcess : public IProcess
{
public:

	explicit TagProcess ()
	{		
		m_executed = false;
	}

	~TagProcess (){};

public:
	bool isExecuted()
	{
		return m_executed;
	}

protected:
	virtual void onUpdate(ant::DeltaTime dt){
		if (!m_executed)
		{
			std::cout << "KABOOOMM!" << std::endl;
			m_executed = true;
			succeedAndStop();
		}
	}

protected:
	bool m_executed;

};

TEST(Test_ProcessManager, basicProcessTest)
{
	// Init manager
	ProcessManager manager;

	// Setup the processes
	IProcessStrongPtr pDelay(new DelayProcess(2)); // 3 secs
	IProcessStrongPtr pTag(new TagProcess()); // 3 secs

	// Build network
	pDelay->attachChild(pTag);
	manager.attachProcess(pDelay);

	ant::DeltaTime time = 0;
	ant::DeltaTime dt = 0.1;
	ant::DeltaTime end = 4;

	// update once!
	manager.updateProcesses(dt);

	// Delay process should be alive now!
	EXPECT_EQ (pDelay->isAlive(),true);

	// Loop in 4 seconds
	while (time < end)
	{
		manager.updateProcesses(dt);
		time += dt;
	}

	// Check things now. First, is tag activated?
	EXPECT_EQ (((TagProcess*)pTag.get())->isExecuted(),true);

	// Is all processes gone?
	EXPECT_EQ (manager.getNumProcesses(),0);

	// Are both processes executed with success?
	EXPECT_EQ (pDelay->getState(),IProcess::SUCCEEDED);
	EXPECT_EQ (pTag->getState(),IProcess::SUCCEEDED);

	// Are both dead?
	EXPECT_EQ (pDelay->isDead(),true);
	EXPECT_EQ (pTag->isDead(),true);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

