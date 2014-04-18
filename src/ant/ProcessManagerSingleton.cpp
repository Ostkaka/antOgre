#include <ant/ProcessManagerSingleton.hpp>
#include <ant/ProcessManager.hpp>

using namespace ant;

ProcessManagerSingleton* ProcessManagerSingleton::s_instance = NULL;

ant::ProcessManagerSingleton::ProcessManagerSingleton()
{
}

void ant::ProcessManagerSingleton::startup(void)
{
	if (s_instance)
	{
		ANT_WARNING("Overwriting ProcessManagerSingleton singleton");
		SAFE_DELETE(s_instance);
	}
	s_instance = ANT_NEW ProcessManagerSingleton;

	if (s_instance->getProcessManager())
	{
		ANT_WARNING("Overwriting process manager on ProcessmangerSingleton");
		SAFE_DELETE(s_instance->m_processManager);
	}
	s_instance->m_processManager = ANT_NEW ProcessManager;
}

void ant::ProcessManagerSingleton::shutdown(void)
{
	ANT_ASSERT(s_instance->m_processManager);
	SAFE_DELETE(s_instance->m_processManager);

	ANT_ASSERT(s_instance);
	SAFE_DELETE(s_instance);
}

ProcessManagerSingleton* ant::ProcessManagerSingleton::instance( void )
{
	return s_instance;
}

ProcessManager* ant::ProcessManagerSingleton::getProcessManager( void )
{
	return m_processManager;
}
