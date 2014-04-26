#include "TestEvents.hpp"

using namespace ant;

const EventType EvtData_Set_Controlled_Actor::sk_EventType(0xbe5e3388);

bool EvtData_Set_Controlled_Actor::buildEventFromScript(void)
{
	if (m_eventData.IsInteger())
	{
		m_id = m_eventData.GetInteger();
		return true;
	}

	return false;
}

void registerTestEvents()
{
	ANT_REGISTER_SCRIPT_EVENT(EvtData_Set_Controlled_Actor, EvtData_Set_Controlled_Actor::sk_EventType);
}