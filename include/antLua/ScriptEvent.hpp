#ifndef SCRIPTEVENT_HPP_
	#define SCRIPTEVENT_HPP_

#include <ant/eventsystem/EventManager.hpp>
#include <LuaPlus.h>

namespace ant
{
	class ScriptEvent;
	typedef ScriptEvent* (*CreateEventForScriptFunctionType)(void); // function ptr typedef to create a script event

	#define ANT_REGISTER_SCRIPT_EVENT(eventClass, eventType) \
		ScriptEvent::registerEventTypeWithScript(#eventClass, eventType); \
		ScriptEvent::addCreationFunction(eventType, &eventClass::createEventForScript)

	#define ANT_EXPORT_FOR_SCRIPT_EVENT(eventClass) \
		public: \
			static ScriptEvent* createEventForScript(void) \
			{ \
				return new eventClass; \
			}

	class ScriptEvent : public BaseEventData
	{
		typedef std::map<EventType, CreateEventForScriptFunctionType> CreationFunctions;
		static CreationFunctions s_creationFunctions;

	public:
		/// Construction
		ScriptEvent(void);

		// This functions is called when event is sent from c++ to script
		LuaPlus::LuaObject getEventData(void);

		// Called when event is sent from script to C++
		bool setEventData(LuaPlus::LuaObject eventData);

		//////////////////////////////////////////////////////////////////////////
		// Static helper functions
		//////////////////////////////////////////////////////////////////////////
		/**
		 * These functions is called by ANT_REGISTER_SCRIPT_EVENT and ANT_EXPORT_FOR_SCRIPT_EVENT
		 * in the class declarations of events exported to lua
		 */
		static void registerEventTypeWithScript(const char* key, EventType type);

		static void addCreationFunction(EventType type, CreateEventForScriptFunctionType pCreationFunctionPtr);
	
		// This is used to transport events from script to c++
		static ScriptEvent* createEventFromScript(EventType type);

		// Clear all the registered events
		static void clearAllRegisterdScriptEvents(void);

	protected:

		virtual void buildEventData(void);

		virtual bool buildEventFromScript(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		LuaPlus::LuaObject m_eventData;

		bool m_eventDataIsValid;
	};
}

#endif