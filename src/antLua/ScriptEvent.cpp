#include <ant/luascripting/ScriptEvent.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/ant_std.hpp>

using namespace ant;

ScriptEvent::CreationFunctions ScriptEvent::s_creationFunctions;

LuaPlus::LuaObject ant::ScriptEvent::getEventData( void )
{
	if (!m_eventDataIsValid)
	{
		buildEventData();
		m_eventDataIsValid = true;
	}

	return m_eventData;
}

ant::ScriptEvent::ScriptEvent( void )
{
	m_eventDataIsValid = false;
}

bool ant::ScriptEvent::setEventData( LuaPlus::LuaObject eventData )
{
	m_eventData = eventData;
	m_eventDataIsValid = buildEventFromScript();
	return m_eventDataIsValid;
}

void ant::ScriptEvent::registerEventTypeWithScript( const char* key, EventType type )
{
	// get or create the EventType table
	LuaPlus::LuaObject eventTypeTable = LuaStateManager::instance()->getGlobalVars().GetByName("EventType");
	if (eventTypeTable.IsNil())
	{
		eventTypeTable = LuaStateManager::instance()->getGlobalVars().CreateTable("EventType");
	}

	// error checking
	GCC_ASSERT(eventTypeTable.IsTable());
//	GCC_ASSERT(eventTypeTable[key].IsNil());

	// add the entry
	eventTypeTable.SetNumber(key, (double)type);
}

void ant::ScriptEvent::addCreationFunction( EventType type, CreateEventForScriptFunctionType pCreationFunctionPtr )
{
	//GCC_ASSERT(s_creationFunctions.find(type) == s_creationFunctions.end());
	GCC_ASSERT(pCreationFunctionPtr != NULL);
	s_creationFunctions.insert(std::make_pair(type, pCreationFunctionPtr));
}

ScriptEvent* ant::ScriptEvent::createEventFromScript( EventType type )
{
	CreationFunctions::iterator findIt = s_creationFunctions.find(type);
	if (findIt != s_creationFunctions.end())
	{
		CreateEventForScriptFunctionType func = findIt->second;
		return func();
	}
	else
	{
		GCC_ERROR("Could not find event type");
		return NULL;
	}
}

void ant::ScriptEvent::clearAllRegisterdScriptEvents( void )
{
	//s_creationFunctions.clear();
}

// This virtual function has to be implemented by base classes to converts data in m_eventData
// to the internal type of event data in c++
bool ant::ScriptEvent::buildEventFromScript( void )
{
	return true;
}

void ScriptEvent::buildEventData( void )
{
	m_eventData.AssignNil(LuaStateManager::instance()->getLuaState());
}