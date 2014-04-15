#include <ant/luascripting/ScriptEventListener.hpp>
#include <ant/luascripting/ScriptEvent.hpp>
#include <ant/eventsystem/IEventManager.hpp>

using namespace ant;

//////////////////////////////////////////////////////////////////////////
// ScriptEventListener
//////////////////////////////////////////////////////////////////////////

ant::ScriptEventListener::ScriptEventListener( const EventType& eventType, const LuaPlus::LuaObject& scriptCallbackFunction )
	:m_scriptCallbackFunction(scriptCallbackFunction)
{
	m_eventType = eventType;
}

ant::ScriptEventListener::~ScriptEventListener()
{
	IEventManager* pEventManager = IEventManager::instance();
	if (pEventManager)
	{
		pEventManager->removeListener(getDelegate(),m_eventType);
	}
}

ant::EventListenerDelegate ant::ScriptEventListener::getDelegate( void )
{
	return MakeDelegate(this, &ScriptEventListener::scriptEventDelegate);
}

void ant::ScriptEventListener::scriptEventDelegate( IEventDataStrongPtr pEvent )
{
	GCC_ASSERT(m_scriptCallbackFunction.IsFunction());

	// Call the lua function
	ScriptEventStrongPtr pScriptEvent = static_pointer_cast<ScriptEvent>(pEvent);	
	LuaPlus::LuaFunction<void> luaCallback = m_scriptCallbackFunction;
	luaCallback(pScriptEvent->getEventData());
}

//////////////////////////////////////////////////////////////////////////
// ScriptEventListenerManager
//////////////////////////////////////////////////////////////////////////

ant::ScriptEventListenerManager::~ScriptEventListenerManager( void )
{
	for (auto it = m_listener.begin() ; it != m_listener.end() ; ++it)
	{
		ScriptEventListener* pListener = (*it);
		delete pListener;
	}
	m_listener.clear();
}

void ant::ScriptEventListenerManager::addListener( ScriptEventListener* pListener )
{
	m_listener.insert(pListener);
}

void ant::ScriptEventListenerManager::destroyListener( ScriptEventListener* pListener )
{
	ScriptEventListenerSet::iterator findIt = m_listener.find(pListener);
	if (findIt != m_listener.end())
	{
		m_listener.erase(findIt);
		delete pListener;
	}
	else
	{
		GCC_ERROR("Couldn't find script listener in set; this will probably cause a memory leak");
	}
}