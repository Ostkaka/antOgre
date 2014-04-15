#ifndef SCRIPTEVENTLISTENER_HPP_
	#define SCRIPTEVENTLISTENER_HPP_

#include <ant/core_types.hpp>
#include <ant/eventsystem/IEventManager.hpp>
#include <set>
#include <LuaPlus.h>

namespace ant
{
	/**
	* This is a C++ listener proxy for script event listeners. Pairs with a single type of event with a
	* lua callback function.
	*/
	class ScriptEventListener
	{
	public:
		/// Default constructor
		explicit ScriptEventListener(const EventType& eventType, const LuaPlus::LuaObject& scriptCallbackFunction);
		
		/// Default deconstructor
		~ScriptEventListener();
		
		/// Returns the delegate of the listener
		EventListenerDelegate getDelegate(void);

		/// Do the event delegate of the listener
		void scriptEventDelegate(IEventDataStrongPtr pEvent);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		/// The lua object that points to a function lua
		LuaPlus::LuaObject m_scriptCallbackFunction;
		/// This is the event-type that the listener listens to
		EventType m_eventType;
	};

	/**
	 * This class manages C++ event listener proxies.
	 */
	class ScriptEventListenerManager
	{
		typedef std::set<ScriptEventListener*> ScriptEventListenerSet;
		
	public:
		/// Default destructor
		~ScriptEventListenerManager(void);

		/// Add a proxy eventListener to the manager
		void addListener(ScriptEventListener* pListener);

		/// Remove a proxy eventListener to the manager
		void destroyListener(ScriptEventListener* pListener);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		/// Set that holds the listeners that serve as proxies to lua listeners
		ScriptEventListenerSet m_listener;
	};
}


#endif