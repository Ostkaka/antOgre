#include <ant/core_types.hpp>
#include <ant/IEventManager.hpp>

using namespace ant;

static IEventManager* g_eventManager = NULL;
GenericObjectFactory<IEventData, EventType> ant::g_EventFactory;

ant::IEventManager::IEventManager( const std::string& name, bool setGlobal )
	:m_name(name)
{
	if (setGlobal)
	{
		if (g_eventManager)
		{
			ANT_WARNING("Attempting to create two global event managers! The old one is destroyed.");
			delete g_eventManager;
		}
		g_eventManager = this;
	}
}

ant::IEventManager::~IEventManager( void )
{
	if (g_eventManager == this)
	{
		g_eventManager = NULL;
	}
}

IEventManager* ant::IEventManager::instance( void )
{
	ANT_ASSERT(g_eventManager);
	return g_eventManager;
}

//////////////////////////////////////////////////////////////////////////
// BaseEventData
//////////////////////////////////////////////////////////////////////////

void ant::BaseEventData::serialize( std::ostrstream &out ) const
{

}

void ant::BaseEventData::deserialize( std::istrstream& in )
{

}
