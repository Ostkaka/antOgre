#ifndef SCRIPTPROCESS_HPP_
	#define SCRIPTPROCESS_HPP_

#include <ant/interfaces/IProcess.hpp>
#include <LuaPlus.h>

namespace ant
{
	/**
	* A process that couples a lua script to a IProcess instance
	*/
	class IScriptProcess : public IProcess
	{
	public:

		static void registerScriptClass(void);

	protected:

		// IProcess interface
		virtual void onInit(void);
		virtual void onUpdate(ant::DeltaTime dt);
		virtual void onSuccess(void);
		virtual void onFail(void);
		virtual void onAbort(void);

	private:
		/// Private helper functions
		static void registerScriptClassFunctions(LuaPlus::LuaObject& metaTableObj);
		
		static LuaPlus::LuaObject createFromScript(LuaPlus::LuaObject self, 
			LuaPlus::LuaObject constructionData,
			LuaPlus::LuaObject originalSubClass);
		
		virtual bool buildCppDataFromScript(LuaPlus::LuaObject scriptClass, 
			LuaPlus::LuaObject constructionData);

		// These are needed because the base-class version of these functions are all const and LuaPlus can't deal
		// with registering const functions.
		bool scriptIsAlive(void);
		bool scriptIsDead(void);
		bool scriptIsPaused(void);

		 // This wrapper function is needed so we can translate a Lua script object to something C++ can use.
		void scriptAttachChild(LuaPlus::LuaObject child);

		// Don't allow construction outside of this class
		explicit IScriptProcess(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		ant::DeltaTime m_frequency;
		ant::DeltaTime m_time;
		LuaPlus::LuaObject m_scriptInitFunction;
		LuaPlus::LuaObject m_scriptUpdateFunction;
		LuaPlus::LuaObject m_scriptSuccessFunction;
		LuaPlus::LuaObject m_scriptFailFunction;
		LuaPlus::LuaObject m_scriptAbortFunction;
		LuaPlus::LuaObject m_self;
	};

	/* Implementation */
	ANT_INLINE bool IScriptProcess::scriptIsAlive(void) {return isAlive();}
	ANT_INLINE bool IScriptProcess::scriptIsDead(void) {return isDead();}
	ANT_INLINE bool IScriptProcess::scriptIsPaused(void) {return isPaused();}
}

#endif