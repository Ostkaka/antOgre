#ifndef EVENTMANAGER_HPP_
	#define EVENTMANAGER_HPP_

#include <ant/eventsystem/IEventManager.hpp>
#include <ant/core_types.hpp>

namespace ant
{

	const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

	typedef std::list<EventListenerDelegate> EventListenerList;
	typedef std::map<EventType, EventListenerList> EventListenerMap;
	typedef std::list<IEventDataStrongPtr> EventQueue;

	/**
	 * Basic implementation of the IEventManager interface
	 */
	class EventManager : public IEventManager
	{
	public:
		/// Constructor
		explicit EventManager(const std::string& name, bool setGlobal);

		/// Destructor
		virtual ~EventManager(void);

		virtual bool addListener(const EventListenerDelegate& eDelegate, const EventType& type);

		virtual bool removeListener(const EventListenerDelegate& eDelegate, const EventType& type);

		virtual bool triggerEvent(const IEventDataStrongPtr& pEvent) const;

		virtual bool queueEvent(const IEventDataStrongPtr& pEvent);

		virtual bool abortEvent(const EventType& type, bool allOfType = false);

		virtual bool update(const ant::DeltaTime& maxDt = EM_INFINITE);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		EventListenerMap m_eventListeners;
		EventQueue m_eventQueues[EVENTMANAGER_NUM_QUEUES];
		int m_activeEventQueue;
	
	};
}

#endif
