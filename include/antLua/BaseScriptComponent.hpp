#ifndef BASESCRIPTCOMPONENT_HPP_
	#define BASESCRIPTCOMPONENT_HPP_

#include <ant/actors/IScriptComponent.hpp>
#include <LuaPlus.h>
//#include <ant/luascripting///.hpp>

namespace ant
{
	/**
	 * Basic implementation of the IScriptComponent interface with use of Lua
	 */
	class BaseScriptComponent : public IScriptComponent
	{
	public:

		BaseScriptComponent(void);
		virtual ~BaseScriptComponent(void);

		/// Inits the script component from XML description
		virtual bool init(TiXmlElement* pData);

		/// Post init function
		virtual void postInit(void);

		/// Generate XML description of the script component
		virtual TiXmlElement* generateXml(void);

		//////////////////////////////////////////////////////////////////////////
		// Statics that registers the script functions
		//////////////////////////////////////////////////////////////////////////
		static void registerScriptFunctions(void);
		static void unregisterScriptFunctions(void);
	
	private:
		void createScriptObject(void);

		// Component script functions
		LuaPlus::LuaObject getActorId(void);
		LuaPlus::LuaObject getPos(void);
		

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////

	public:

		static const char* g_Name;
		virtual const char* getName() const { return g_Name; } 

	protected:
		/// Following variables are functions names in lua declared by the xml
		std::string m_scriptObjectName;
		std::string m_constructorName;
		std::string m_destructorName;

		LuaPlus::LuaObject m_scriptObject;
		LuaPlus::LuaObject m_scriptConstructorObject;
		LuaPlus::LuaObject m_scriptDestructorObject;
	};

	/// Implementation
	//ANT_INLINE const std::string& BaseScriptComponent::getName() const { return g_Name; }
}

#endif