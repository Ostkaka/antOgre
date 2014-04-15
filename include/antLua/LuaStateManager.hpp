#ifndef LUASTATEMANAGER_HPP_
	#define LUASTATEMANAGER_HPP_

#include <ant/core_types.hpp>
#include <ant/luascripting/IScriptManager.hpp>
#include <SFML/Graphics.hpp>
#include <LuaPlus.h>

namespace ant
{
	/**
	* Singleton class that handles a LuaState.
	*/
	class LuaStateManager : public IScriptManager
	{
	public:
		
		// IScriptManager interface
		virtual bool init(void) ANT_OVERRIDE;

		/// Executes a file from a given path in the active lua state
		virtual void executeFile(const char* path) ANT_OVERRIDE;
		
		/// Executes a string in the active lua state
		virtual void executeString(const char* str) ANT_OVERRIDE;
		
		/// Create the internal lua state
		static bool create(void);

		/// Clear the internal lua state
		static void destroy(void);

		/// Gets the instance 
		static LuaStateManager* instance(void);
		
		/// Get the global vars in the lua state contained in the manager
		LuaPlus::LuaObject getGlobalVars(void);
		
		/// Gets the internal lua state contained in the manager
		LuaPlus::LuaState* getLuaState(void) const;
		
		/// Helper functions
		LuaPlus::LuaObject createPath(const std::string& path, bool ignoreLastElement = false);

		// TODO - Vec2
		void convertVec2ToTable(const sf::Vector2f& vec, LuaPlus::LuaObject& vec2Table);

	private:
		void setError(int errorNum);
		void clearStack(void);

		/// Private functions. Should use Create() and Destroy().
		explicit LuaStateManager(void);
		virtual ~LuaStateManager(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		static LuaStateManager* s_pInstance;
		LuaPlus::LuaState* m_pLuaState;
		std::string m_lastError;
	};

	/* Implementation */
	ANT_INLINE LuaStateManager * LuaStateManager::instance()
	{
		GCC_ASSERT(s_pInstance);
		return s_pInstance;
	}
}

#endif