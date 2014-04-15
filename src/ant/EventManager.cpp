#include <ant/eventsystem/EventManager.hpp>
#include <ant/core_types.hpp>
#include <ant/gccUtils/String.hpp>
#include <SFML/System/Clock.hpp>
#include <ant/gccDebug/Logger.hpp>

using namespace ant;

ant::EventManager::EventManager( const std::string& name, bool setGlobal )
	: IEventManager(name,setGlobal)
{
	m_activeEventQueue = 0;
}

ant::EventManager::~EventManager( void )
{
}

bool ant::EventManager::addListener( const EventListenerDelegate& eDelegate, const EventType& type )
{
	GCC_LOG("Events", "Attempting to add delegate function for event type: " + ToStr(type, 16));

	EventListenerList& eventListenerList = m_eventListeners[type];  

	for (auto it = eventListenerList.begin() ; it != eventListenerList.end() ; ++it)
	{
		if (eDelegate == (*it))
		{
			GCC_WARNING("Trying to double add an eventListener");
			return false;
		}
	}
	eventListenerList.push_back(eDelegate);
	GCC_LOG("Events","Successfully added delegate for event type:" + ToStr(type,16));
	return true;
}

bool ant::EventManager::removeListener( const EventListenerDelegate& eDelegate, const EventType& type )
{
	GCC_LOG("Events", "Attempting to remove delegate function for event type: " + ToStr(type, 16));

	bool succ = false;

	auto findIt = m_eventListeners.find(type);
	if (findIt != m_eventListeners.end())
	{
		EventListenerList& listeners = findIt->second;
		for(auto it = listeners.begin() ; it != listeners.end() ; ++it)
		{

			if (eDelegate == (*it))
			{
				listeners.erase(it);
				GCC_LOG("Events", "Successfully removed delegate function from event type: " + ToStr(type, 16));
				succ = true;
				break;
			}
		}
	}
	return succ;
}

bool ant::EventManager::triggerEvent( const IEventDataStrongPtr& pEvent ) const
{
	GCC_LOG("Events", "Attempting to trigger event " + pEvent->getName());
	bool processedEvent = false;

	auto findIt = m_eventListeners.find(pEvent->getEventType());
	if (findIt != m_eventListeners.end())
	{
		const EventListenerList& elList = findIt->second;
		for (auto it = elList.begin() ; it != elList.end() ; ++it)
		{
			EventListenerDelegate listener = (*it);
			GCC_LOG("Events", "Sending Event " + pEvent->getName() + " to delegate function");
			listener(pEvent);
			processedEvent = true;
		}
	}
	return processedEvent;
}

bool ant::EventManager::queueEvent( const IEventDataStrongPtr& pEvent )
{
	// Assert that the variables are OK
	GCC_ASSERT(m_activeEventQueue >= 0);
	GCC_ASSERT(m_activeEventQueue < EVENTMANAGER_NUM_QUEUES);

	// is the event valid
	if (!pEvent)
	{
		GCC_ERROR("Passed event is invalid in queueEvent()");
		return false;
	}

	GCC_LOG("Events", "Attempting to queue event: " + pEvent->getName());

	auto elIt = m_eventListeners.find(pEvent->getEventType());
	if (elIt != m_eventListeners.end())
	{
		m_eventQueues[m_activeEventQueue].push_back(pEvent);
		GCC_LOG("Events","Succsessfully queued event: " + pEvent->getName());
		return true;
	}
	else
	{
		GCC_LOG("Events","No eventlistener delegates are registerd for: " + pEvent->getName() + " , skipping it");
		return false;
	}
}

bool ant::EventManager::abortEvent( const EventType& type, bool allOfType /*= false*/ )
{
	GCC_ASSERT(m_activeEventQueue >= 0);
	GCC_ASSERT(m_activeEventQueue < EVENTMANAGER_NUM_QUEUES);

	bool succ = false;
	EventListenerMap::iterator findIt = m_eventListeners.find(type);

	if (findIt != m_eventListeners.end())
	{
		EventQueue& eventQueue = m_eventQueues[m_activeEventQueue];
		auto it = eventQueue.begin();
		while (it != eventQueue.end())
		{
			
			// Can't remove since it will invalidate the iterator
			auto thisIt = it;
			++it;

			if ((*thisIt)->getEventType() == type)
			{
				eventQueue.erase(thisIt);
				succ = true;
				// Continue if allOfType is specified
				if (!allOfType)
					break;
			}
		}
	}

	return succ;
}

bool ant::EventManager::update( const ant::DeltaTime& maxDt /*= EM_INFINITE*/ )
{
	sf::Clock eventClock;
	ant::DeltaTime currTime = eventClock.restart().asSeconds();
	ant::DeltaTime maxMs = ((maxDt == EventManager::EM_INFINITE ? (EventManager::EM_INFINITE) : (maxDt + currTime)));

	// Could include real-time events here - Do this later


	// Swap active queues and clear the old queue
	int queueToProcess = m_activeEventQueue;
	m_activeEventQueue = (m_activeEventQueue +1) % EVENTMANAGER_NUM_QUEUES;
	m_eventQueues[m_activeEventQueue].clear();

	GCC_LOG("EventManager", "Processing Event Queue " + ToStr(queueToProcess) + "; " + ToStr((unsigned long)m_eventQueues[queueToProcess].size()) + " events to process");

	// Process the queue until it's empty
	while(!m_eventQueues[queueToProcess].empty())
	{
		// Pop front
		IEventDataStrongPtr pEvent = m_eventQueues[queueToProcess].front();
		m_eventQueues[queueToProcess].pop_front();

		GCC_LOG("EventManager", "\t\tProcessing Event " + (pEvent->getName()));

		const EventType& eventType = pEvent->getEventType();

		// find the delegate functions registered for this event
		auto findIt = m_eventListeners.find(eventType);
		if (findIt != m_eventListeners.end())
		{
			const EventListenerList& eventListeners = findIt->second;
			GCC_LOG("EventManager", "\t\tFound " + ToStr((unsigned long)eventListeners.size()) + " delegates");

			// Loop every listener delegate and and execute the delegate
			for (auto it = eventListeners.begin() ; it != eventListeners.end() ; ++it)
			{
				EventListenerDelegate listener = (*it);
				GCC_LOG("EventManager", "\t\tSending event " + (pEvent->getName()) + " to delegate");
				listener(pEvent);
			}
		}

		// Check if time ran out
		currTime = eventClock.getElapsedTime().asSeconds();
		if (maxMs != IEventManager::EM_INFINITE && currTime >= maxMs)
		{
			GCC_LOG("EventManager","Aborting event processing due to timeout");
			break;
		}
	}

	// Send all events that could not be processed this update state to the next queue to be handled
	// in the next update
	bool queueEmpty = (m_eventQueues[queueToProcess].empty());
	if (!queueEmpty)
	{
		while (!m_eventQueues[queueToProcess].empty())
		{
			IEventDataStrongPtr pEvent = m_eventQueues[queueToProcess].back();
			m_eventQueues[queueToProcess].pop_back();
			m_eventQueues[m_activeEventQueue].push_front(pEvent);
		}
	}

	return queueEmpty;
}
