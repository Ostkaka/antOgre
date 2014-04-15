#include <ant/luascripting/LuaScriptExports.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/resources/ResourceCacheManager.hpp>
#include <ant/classes/ProcessManagerSingleton.hpp>
#include <ant/classes/ProcessManager.hpp>
#include <ant/interfaces/IProcess.hpp>
#include <ant/resources/ResourceCache.hpp>
#include <ant/luascripting/ScriptEventListener.hpp>
#include <ant/luascripting/ScriptEvent.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/core_types.hpp>
#include <ant/ant_std.hpp>
#include <SFML/Graphics.hpp>
#include <ant/interfaces/ISFMLApp.hpp>
#include <ant/eventsystem/Events.hpp>
#include <ant/interfaces/IGameLogic.hpp>
#include <ant/actors/Actor.hpp>
#include <LuaPlus.h>

using namespace ant;

void LuaScriptExports::registerScripts( void )
{
	LuaPlus::LuaObject globals = LuaStateManager::instance()->getGlobalVars();

	// Init
	InternalLuaScriptExports::init();

	// Resource loading
	globals.RegisterDirect("loadAndExecuteScriptResource",InternalLuaScriptExports::loadAndExecutreScriptResource);	

	// Process manager
	globals.RegisterDirect("attachProcess",&InternalLuaScriptExports::attachScriptProcess);

	// Event system
	globals.RegisterDirect("registerEventListener", &InternalLuaScriptExports::registerEventListener);
	globals.RegisterDirect("removeEventListener", &InternalLuaScriptExports::removeEventListener);
	globals.RegisterDirect("queueEvent", &InternalLuaScriptExports::queueEvent);
	globals.RegisterDirect("triggerEvent", &InternalLuaScriptExports::triggerEvent);

	// Actors
	globals.RegisterDirect("createActor", &InternalLuaScriptExports::createActor);

	// Lua log
	globals.RegisterDirect("log",&InternalLuaScriptExports::lualog);
}

void LuaScriptExports::unregisterScripts( void )
{
	InternalLuaScriptExports::destroy();
}

//////////////////////////////////////////////////////////////////////////
// InternalLuaScriptExports
//////////////////////////////////////////////////////////////////////////

ScriptEventListenerManager* InternalLuaScriptExports::s_ScriptEventListenerMgrInstance = NULL;

bool ant::InternalLuaScriptExports::init( void )
{
	GCC_ASSERT(s_ScriptEventListenerMgrInstance == NULL);
	s_ScriptEventListenerMgrInstance = GCC_NEW ScriptEventListenerManager;

	return true;
}

void ant::InternalLuaScriptExports::destroy( void )
{
	GCC_ASSERT(s_ScriptEventListenerMgrInstance != NULL);
	SAFE_DELETE(s_ScriptEventListenerMgrInstance);
}

bool ant::InternalLuaScriptExports::loadAndExecutreScriptResource( const char* scriptResource )
{
	// Check if cache is loaded
	if (!ResourceCacheManager::instance()->getResourceCache())
	{
		GCC_WARNING("Tried to access cache in CacheManager, but it was a bad pointer");
		return false;
	}
	
	if (!ResourceCacheManager::instance()->getResourceCache()->isUsingDevelopmentDirectories())
	{
		Resource resource(scriptResource);
		// this actually loads the Lua file from the zip file
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);  
		if (pResourceHandle)
		{
			return true;
		}
		return false;
	}
	else
	{
		std::string path(ANT_DATA_PATH"\\"); // TODO
		path += scriptResource;
		LuaStateManager::instance()->executeFile(path.c_str());
		return true;
	}
	
	return true;
}

void ant::InternalLuaScriptExports::attachScriptProcess( LuaPlus::LuaObject scriptProcess )
{
	LuaPlus::LuaObject temp = scriptProcess.Lookup("__object");
	if (!temp.IsNil())
	{
		IProcessStrongPtr pProcess(static_cast<IProcess*>(temp.GetLightUserData()));
		ProcessManagerSingleton::instance()->getProcessManager()->attachProcess(pProcess);
	}
	else
	{
		GCC_ERROR("Could not find __object in script proces");
	}
}

bool ant::InternalLuaScriptExports::queueEvent( EventType eventType, LuaPlus::LuaObject eventData )
{
	ScriptEventStrongPtr pEvent(buildEvent(eventType,eventData));
	if (pEvent)
	{
		IEventManager::instance()->queueEvent(pEvent);
		return true;
	}
	return false;
}

bool ant::InternalLuaScriptExports::triggerEvent( EventType eventType, LuaPlus::LuaObject eventData )
{
	// Set the event data that was passed in
	ScriptEventStrongPtr pEvent(buildEvent(eventType,eventData));
	if (pEvent)
	{
		return IEventManager::instance()->triggerEvent(pEvent);
	}
	return false;
}

void ant::InternalLuaScriptExports::lualog( LuaPlus::LuaObject text )
{
	if (text.IsConvertibleToString())
	{
		GCC_LOG("Lua",text.ToString());
	}
	else
	{
		GCC_LOG("Lua","<" + std::string(text.TypeName()) + ">");
	}
}

ant::ScriptEventStrongPtr ant::InternalLuaScriptExports::buildEvent( EventType eventType, LuaPlus::LuaObject eventData )
{
	// Create the event from the event type
	ScriptEventStrongPtr pEvent(ScriptEvent::createEventFromScript(eventType));
	if (!pEvent)
	{
		return ScriptEventStrongPtr();
	}

	// Set the event data that was sent in. Return empty pointer if fails.
	if (!pEvent->setEventData(eventData))
	{
		return ScriptEventStrongPtr();
	}

	return pEvent;
}

ant::Ulong ant::InternalLuaScriptExports::registerEventListener( EventType eventType, LuaPlus::LuaObject callbackFunction )
{
	GCC_ASSERT(s_ScriptEventListenerMgrInstance);

	if (callbackFunction.IsFunction())
	{
		// Create the C++ listener proxy and set it to listen for the event
		ScriptEventListener* pListener = GCC_NEW ScriptEventListener(eventType, callbackFunction);
		s_ScriptEventListenerMgrInstance->addListener(pListener);
		IEventManager::instance()->addListener(pListener->getDelegate(), eventType);

		// Convert the pointer to an unsigned log to use as a handle
		ant::Ulong handle = reinterpret_cast<ant::Ulong>(pListener);
		return handle;
	}

	GCC_ERROR("Attempting to register script event listener with invalid callback function");
	return 0;
}

void ant::InternalLuaScriptExports::removeEventListener( ant::Ulong listenerId )
{
	GCC_ASSERT(s_ScriptEventListenerMgrInstance);
	GCC_ASSERT(listenerId != 0);

	// Convert the listener id back to a pointer
	ScriptEventListener* pListener = reinterpret_cast<ScriptEventListener*>(listenerId);
	// the destructor will remove the listener
	s_ScriptEventListenerMgrInstance->destroyListener(pListener); 

	// TODO Err, should not the listener be removed form the global event listener here here?
}

int ant::InternalLuaScriptExports::createActor( const char* actorArchetype, LuaPlus::LuaObject luaPosition, LuaPlus::LuaObject luaRotation )
{
	if (!luaPosition.IsTable())
	{
		GCC_ERROR("Invalid object passed to createActor(); type = " + std::string(luaPosition.TypeName()));
		return INVALID_ACTOR_ID;
	}

	if (!luaRotation.IsNumber())
	{
		GCC_ERROR("Invalid object passed to createActor(); type = " + std::string(luaRotation.TypeName()));
		return INVALID_ACTOR_ID;
	}

	sf::Vector2f pos(luaPosition["x"].GetFloat(), luaPosition["y"].GetFloat());
	ant::Real rot = luaRotation.GetFloat();

	TiXmlElement *overloads = NULL;
	ActorStrongPtr actor = ant::ISFMLApp::getApp()->getGameLogic()->createActor(actorArchetype, overloads, &pos, &rot);

	if (actor)
	{
		shared_ptr<EvtData_New_Actor> pNewActorEvent(GCC_NEW EvtData_New_Actor(actor->getId()));
		IEventManager::instance()->queueEvent(pNewActorEvent);
		return actor->getId();
	}

	return INVALID_ACTOR_ID;
}

