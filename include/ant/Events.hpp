#ifndef EVENTS_HPP_
	#define EVENTS_HPP_

#include <ant/IEventManager.hpp>
#include <antLua/ScriptEvent.hpp>
#include <antLua/LuaStateManager.hpp>
#include <ant/core_types.hpp>

namespace ant
{
	
	/// Registers the event types to script
	void registerEngineScriptEvents( void );

	/**
	 * This event is sent by the game logic each tick.
	 */
	class EvtData_Update_Tick : public BaseEventData
	{
		static const EventType sk_EventType;

		explicit EvtData_Update_Tick( const ant::DeltaTime dt)
			:m_deltaTime(dt)
		{
		}

		virtual const EventType& getEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy() const
		{
			return IEventDataStrongPtr (ANT_NEW EvtData_Update_Tick ( m_deltaTime ) );
		}

		virtual void serialize( std::ostrstream & out )
		{
			ANT_ERROR("You should not be serializing update ticks!");
		}

		virtual std::string getName(void) const
		{
			return "EvtData_Update_Tick";
		}

	protected:
		ant::DeltaTime m_deltaTime; 
	};

	//////////////////////////////////////////////////////////////////////////
	// EvtData_New_Actor - Event that is fired when an actor is created
	//////////////////////////////////////////////////////////////////////////
	class EvtData_New_Actor : public BaseEventData
	{
		ActorId m_actorId;
		GameViewId m_viewId;

	public:
		static const EventType sk_EventType;

		EvtData_New_Actor(void)
		{
			m_actorId = INVALID_ACTOR_ID,
			m_viewId = ant_InvalidGameViewId;
		}

		explicit EvtData_New_Actor(ActorId actorId, GameViewId viewId = ant_InvalidGameViewId)
			:m_actorId(actorId), m_viewId(viewId)
		{
		}

		virtual IEventDataStrongPtr copy(void) const
		{
			return IEventDataStrongPtr(ANT_NEW EvtData_New_Actor(m_actorId, m_viewId));
		}

		virtual const EventType& getEventType(void) const
		{
			return sk_EventType;
		}

		virtual void deserialize(std::istrstream& in)
		{
			in >> m_actorId;
			in >> m_viewId;
		}

		virtual void serialize(std::ostrstream& out) const
		{
			out << m_actorId << "	";
			out << m_viewId << "	";
		}

		virtual std::string getName(void) const
		{
			return "EvtData_New_Actor";
		}

		const ActorId getActorId(void) const
		{
			return m_actorId;
		}

		GameViewId getViewId(void) const
		{
			return m_viewId;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// EvtData_Environment_Loaded - this event is sent when a new game is started
	//////////////////////////////////////////////////////////////////////////
	class EvtData_Environment_Loaded : public BaseEventData
	{
	public:
		static const EventType sk_EventType;

		EvtData_Environment_Loaded(void)
		{
		}

		virtual const EventType& getEventType(void) const	{ return sk_EventType; }
		
		virtual IEventDataStrongPtr copy(void) const
		{ 
			return IEventDataStrongPtr( ANT_NEW EvtData_Environment_Loaded( ) ); 
		}
		
		virtual std::string getName(void) const  
		{ 
			return "EvtData_Environment_Loaded";  
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// EvtData_Destroy_Actor - sent when actors are destroyed	
	//////////////////////////////////////////////////////////////////////////
	class EvtData_Destroy_Actor : public BaseEventData
	{
		ActorId m_id;

	public:
		static const EventType sk_EventType;

		explicit EvtData_Destroy_Actor(ActorId id = INVALID_ACTOR_ID)
			: m_id(id)
		{
		}

		virtual const EventType& getEventType(void) const
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy(void) const
		{
			return IEventDataStrongPtr ( ANT_NEW EvtData_Destroy_Actor ( m_id ) );
		}

		virtual void serialize(std::ostrstream &out) const
		{
			out << m_id;
		}

		virtual void deserialize(std::istrstream& in)
		{
			in >> m_id;
		}

		virtual std::string getName(void) const
		{
			return "EvtData_Destroy_Actor";
		}

		ActorId getId(void) const { return m_id; }
	};

	//////////////////////////////////////////////////////////////////////////
	// EvtData_ReloadLevel 
	//////////////////////////////////////////////////////////////////////////
	class EvtData_ReloadLevel : public BaseEventData
	{

	public:
		static const EventType sk_EventType;

		EvtData_ReloadLevel(void) 
		{
		}

		virtual void serialize(std::ostrstream& out) const 
		{
			ANT_ERROR(getName() + std::string(" should not be serialzied!"));
		}

		virtual void deserialize(std::istrstream& in) 
		{
			ANT_ERROR(getName() + std::string(" should not be serialzied!"));
		}

		virtual const EventType& getEventType(void) const
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy(void) const
		{
			return IEventDataStrongPtr(ANT_NEW EvtData_ReloadLevel());
		}

		virtual std::string getName(void) const 
		{
			return "EvtData_ReloadLevel";
		}
	};

#pragma region Unittest_events

	//////////////////////////////////////////////////////////////////////////
	//
	// UNITTEST events
	// 
	//////////////////////////////////////////////////////////////////////////

	/**
	 * This is a test event used by the unit test to verify the integrity of the system.
	 */
	class EvtData_TestExecute : public ant::ScriptEvent
	{

	public:
		static const EventType sk_EventType;

		explicit EvtData_TestExecute()
			: m_executed(false)
		{		
		}

		virtual const EventType& getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy(void) const ANT_OVERRIDE
		{
			return IEventDataStrongPtr ( ANT_NEW EvtData_TestExecute ( ) );
		}

		virtual void serialize(std::ostrstream &out) const ANT_OVERRIDE
		{
			out << m_executed;
		}

		virtual void deserialize(std::istrstream& in) ANT_OVERRIDE
		{
			in >> m_executed;
		}

		virtual std::string getName( void ) const ANT_OVERRIDE
		{
			return "EvtData_Destroy_Actor";
		}

		virtual bool buildEventFromScript( void )
		{
			if (m_eventData.IsBoolean())
			{
				m_executed = m_eventData.GetBoolean();
				return true;
			}
			return false;
		}

		virtual void buildEventData( void )
		{
			m_eventData.AssignNewTable(LuaStateManager::instance()->getLuaState());
			m_eventData.SetBoolean("m_executed", m_executed);				
		}

		void execute(){ m_executed = true;}

		bool isExecuted( void ) const { return m_executed; }

		ANT_EXPORT_FOR_SCRIPT_EVENT(EvtData_TestExecute);

	protected:
		bool m_executed;
	};

	/**
	 * Verify the Lua/C++ boundary communication
	 */
	class EvtData_TestToLua : public ant::ScriptEvent
	{

	public:
		static const EventType sk_EventType;

		explicit EvtData_TestToLua()
			: m_number(0)
		{		
		}

		virtual const EventType& getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy(void) const ANT_OVERRIDE
		{
			return IEventDataStrongPtr ( ANT_NEW EvtData_TestToLua ( ) );
		}

		virtual void serialize(std::ostrstream &out) const ANT_OVERRIDE
		{
			out << m_number;
		}

		virtual void deserialize(std::istrstream& in) ANT_OVERRIDE
		{
			in >> m_number;
		}

		virtual std::string getName( void ) const ANT_OVERRIDE
		{
			return "EvtData_Destroy_Actor";
		}

		virtual void buildEventData( void )
		{
			m_eventData.AssignNewTable(LuaStateManager::instance()->getLuaState());
			m_eventData.SetInteger("m_number", m_number);				
		}

		void add(){ m_number++; }

		int getNumber( void ) const { return m_number; }

		ANT_EXPORT_FOR_SCRIPT_EVENT(EvtData_TestToLua);

	protected:
		int m_number;
	};

	/**
	 * Verify the Lua/C++ boundary communication
	 */
	class EvtData_TestFromLua : public ant::ScriptEvent
	{

	public:
		static const EventType sk_EventType;

		explicit EvtData_TestFromLua()
			: m_number(0)
		{		
		}

		virtual const EventType& getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy(void) const ANT_OVERRIDE
		{
			return IEventDataStrongPtr ( ANT_NEW EvtData_TestToLua ( ) );
		}

		virtual void serialize(std::ostrstream &out) const ANT_OVERRIDE
		{
			out << m_number;
		}

		virtual void deserialize(std::istrstream& in) ANT_OVERRIDE
		{
			in >> m_number;
		}

		virtual std::string getName( void ) const ANT_OVERRIDE
		{
			return "EvtData_Destroy_Actor";
		}

		virtual bool buildEventFromScript( void )
		{
			if (m_eventData.IsTable())
			{
				if (m_eventData.GetByName("m_number").IsInteger())
				{
					m_number = m_eventData.GetByName("m_number").GetInteger();
					return true;
				}
			}
			return false;
		}

		void add(){ m_number++; }

		int getNumber( void ) const { return m_number; }

		ANT_EXPORT_FOR_SCRIPT_EVENT(EvtData_TestFromLua);

	protected:
		int m_number;
	};
}

#pragma endregion

#endif