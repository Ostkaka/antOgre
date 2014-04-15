#include <ant/classes/ProcessManagerSingleton.hpp>
#include <ant/classes/ProcessManager.hpp>

using namespace ant;

ProcessManagerSingleton* ProcessManagerSingleton::s_instance = NULL;

ant::ProcessManagerSingleton::ProcessManagerSingleton()
{
	m_processManager = NULL;
}

ant::ProcessManagerSingleton::~ProcessManagerSingleton()
{
	clearProcessManager();
}

void ant::ProcessManagerSingleton::clearProcessManager( void )
{
	if (m_processManager)
	{
		SAFE_DELETE(m_processManager);
	}
}

void ant::ProcessManagerSingleton::create( void )
{
	if (s_instance)
	{
		GCC_WARNING("Overwriting ProcessManagerSingleton singleton");
		SAFE_DELETE(s_instance);
	}
	s_instance = GCC_NEW ProcessManagerSingleton;
}

void ant::ProcessManagerSingleton::destroy( void )
{
	GCC_ASSERT(s_instance);
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

void ant::ProcessManagerSingleton::init( void )
{
	if (m_processManager)
	{
		GCC_WARNING("Overwriting process manager on ProcessmangerSingleton");
		SAFE_DELETE(m_processManager);
	}
	m_processManager = GCC_NEW ProcessManager;
}
