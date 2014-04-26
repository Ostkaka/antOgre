#include <ant/Events.hpp>
#include <antLua/ScriptEventListener.hpp>

using namespace ant;

/**
 * This is the declaration of the unique GUID:s of the event
 */
const EventType EvtData_Update_Tick::sk_EventType(0xf0f5d183);
const EventType EvtData_Environment_Loaded::sk_EventType(0x3673e27e);
const EventType EvtData_Destroy_Actor::sk_EventType(0x372cc986);
const EventType EvtData_New_Actor::sk_EventType(0x50a51f52);
const EventType EvtData_ReloadLevel::sk_EventType(0x53c1a87);
const EventType EvtData_New_RenderComponent::sk_EventType(0x53d1b87);
	
/**
 * Unit-test events
 */
const EventType EvtData_TestExecute::sk_EventType(0xa3814acd);
const EventType EvtData_TestToLua::sk_EventType(0x5b14c8dc);
const EventType EvtData_TestFromLua::sk_EventType(0x288707ca);

void ant::registerEngineScriptEvents( void )
{
	ANT_REGISTER_SCRIPT_EVENT(EvtData_TestExecute, EvtData_TestExecute::sk_EventType);
	ANT_REGISTER_SCRIPT_EVENT(EvtData_TestToLua, EvtData_TestToLua::sk_EventType);
	ANT_REGISTER_SCRIPT_EVENT(EvtData_TestFromLua, EvtData_TestFromLua::sk_EventType);
}
