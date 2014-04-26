#ifndef TESTEVENTS_HPP_
	#define TESTEVENTS_HPP_

#include <antLua\ScriptEvent.hpp>

void registerTestEvents();

namespace ant
{
	//////////////////////////////////////////////////////////////////////////
	// EvtData_Set_Controlled_Actor - Event that is fired to set a controller to an actor
	//////////////////////////////////////////////////////////////////////////
	class EvtData_Set_Controlled_Actor : public ant::ScriptEvent
	{
		ant::ActorId m_id;

	public:
		static const ant::EventType sk_EventType;

		EvtData_Set_Controlled_Actor(void) { }
		EvtData_Set_Controlled_Actor(ant::ActorId actorId)
			: m_id(actorId)
		{
		}

		virtual const ant::EventType& getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		virtual ant::IEventDataStrongPtr copy() const ANT_OVERRIDE
		{
			return ant::IEventDataStrongPtr(ANT_NEW EvtData_Set_Controlled_Actor(m_id));
		}

		virtual void serialize(std::ostrstream& out) const ANT_OVERRIDE
		{
			out << m_id;
		}

		virtual void deserialize(std::istrstream& in) ANT_OVERRIDE
		{
			in >> m_id;
		}

		const ant::ActorId& getActorId(void) const
		{
			return m_id;
		}

		virtual std::string getName(void) const ANT_OVERRIDE
		{
			return "EvtData_Set_Controlled_Actor";
		}

		virtual bool buildEventFromScript(void) ANT_OVERRIDE;

		ANT_EXPORT_FOR_SCRIPT_EVENT(EvtData_Set_Controlled_Actor);
	};
}

#endif