#ifndef LUASCRIPTEXPORTS_HPP_
	#define LUASCRIPTEXPORTS_HPP_

#include <ant/eventsystem/EventManager.hpp>
#include <ant/luascripting/ScriptEventListener.hpp>
#include <LuaPlus.h>

namespace ant
{

	namespace LuaScriptExports
	{
		void registerScripts(void);
		void unregisterScripts(void);
	}

	/**
	 * This class handles the internal script exports to lua from the game framework
	 */
	class InternalLuaScriptExports
	{
	public:

		/// Initialization
		static bool init(void);
		static void destroy(void);

		/// Export to lua
		static bool loadAndExecutreScriptResource(const char* scriptResource);

		/// Process Manager
		static void attachScriptProcess(LuaPlus::LuaObject scriptProcess);
	
		// Events
		static bool queueEvent(EventType eventType, LuaPlus::LuaObject eventData);
		static bool triggerEvent(EventType eventType, LuaPlus::LuaObject eventData);
		static ant::Ulong registerEventListener(EventType eventType, LuaPlus::LuaObject callbackFunction);
		static void removeEventListener(ant::Ulong listenerId);

		// Actors
		static int createActor(const char* actorArchetype, LuaPlus::LuaObject luaPosition, LuaPlus::LuaObject luaRotation);

		// Lua log
		static void lualog(LuaPlus::LuaObject text);

		/* Add new export functions as the development progresses */

	private:
		static ScriptEventStrongPtr buildEvent(EventType eventType, LuaPlus::LuaObject eventData);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		static ScriptEventListenerManager* s_ScriptEventListenerMgrInstance;
	};
}

#endif