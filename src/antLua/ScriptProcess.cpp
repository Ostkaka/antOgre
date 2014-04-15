#include <ant/luascripting/ScriptProcess.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/core_types.hpp>

const char* SCRIPT_PROCESS_NAME = "ScriptProcess";

ant::IScriptProcess::IScriptProcess( void )
{
	LuaPlus::LuaState* pLuaState = LuaStateManager::instance()->getLuaState();

	m_frequency = 0;
	m_time = 0;
	m_scriptInitFunction.AssignNil(pLuaState);
	m_scriptUpdateFunction.AssignNil(pLuaState);
	m_scriptSuccessFunction.AssignNil(pLuaState);
	m_scriptFailFunction.AssignNil(pLuaState);
	m_scriptAbortFunction.AssignNil(pLuaState);
}

bool ant::IScriptProcess::buildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData )
{
	if (scriptClass.IsTable())
	{
		// OnInit
		LuaPlus::LuaObject temp = scriptClass.GetByName("onInit");
		if (temp.IsFunction())
		{
			m_scriptInitFunction = temp;
		}

		//OnUpdate
		temp = scriptClass.GetByName("onUpdate");
		if (temp.IsFunction())
		{
			m_scriptUpdateFunction = temp;
		}
		else
		{
			GCC_ERROR("No onUpdate() found in the script process; type = " + std::string(temp.TypeName()));
			return false;
		}

		// onSuccess
		temp = scriptClass.GetByName("onSuccess");
		if (temp.IsFunction())
		{
			m_scriptSuccessFunction = temp;
		}

		// onFail()
		temp = scriptClass.GetByName("onFail");
		if (temp.IsFunction())
		{
			m_scriptFailFunction = temp;
		}

		// onAbort()
		temp = scriptClass.GetByName("onAbort");
		if (temp.IsFunction())
		{
			m_scriptAbortFunction = temp;
		}
	}
	else
	{
		GCC_ERROR("scriptClass is not a table in ScriptProcess::BuildCppDataFromScript()");
		return false;
	}

	if (constructionData.IsTable())
	{
		for (LuaPlus::LuaTableIterator constructionDataIt(constructionData) ; constructionDataIt; constructionDataIt.Next())
		{
			const char* key = constructionDataIt.GetKey().GetString();
			LuaPlus::LuaObject val = constructionDataIt.GetValue();

			if (strcmp(key, "frequency") == 0 && val.IsNumber())
			{
				m_frequency = val.GetFloat();
			}
			else
			{
				m_self.SetObject(key, val);
			}
		}
	}
	return true;
}

void ant::IScriptProcess::onInit( void )
{
	IProcess::onInit();
	if (!m_scriptInitFunction.IsNil())
	{
		LuaPlus::LuaFunction<void> func(m_scriptInitFunction);
		func(m_self);
	}

	/// If we have no update function, bail!
	if (!m_scriptUpdateFunction.IsFunction())
	{
		failAndStop();
	}
}

void ant::IScriptProcess::onUpdate( ant::DeltaTime dt )
{
	m_time += dt;
	if (m_time >= m_frequency)
	{
		LuaPlus::LuaFunction<void> func(m_scriptUpdateFunction);
		func(m_self, m_time);
		m_time = 0;
	}
}

void ant::IScriptProcess::onSuccess( void )
{
	if (!m_scriptSuccessFunction.IsNil())
	{
		LuaPlus::LuaFunction<void> func(m_scriptSuccessFunction);
		func(m_self);
	}
}

void ant::IScriptProcess::onFail( void )
{
	if (!m_scriptFailFunction.IsNil())
	{
		LuaPlus::LuaFunction<void> func(m_scriptFailFunction);
		func(m_self);
	}
}

void ant::IScriptProcess::onAbort( void )
{
	if (!m_scriptAbortFunction.IsNil())
	{
		LuaPlus::LuaFunction<void> func(m_scriptAbortFunction);
		func(m_self);
	}
}

void ant::IScriptProcess::scriptAttachChild( LuaPlus::LuaObject child )
{
	if (child.IsTable())
	{
		LuaPlus::LuaObject obj = child.GetByName("__object");
		if (!obj.IsNil())
		{
			// Casting a raw ptr to a smart ptr is generally bad, but Lua has no concept of what a shared_ptr 
			// is.  There's no easy way around it.
			shared_ptr<IProcess> pProcess(static_cast<IProcess*>(obj.GetLightUserData()));
			GCC_ASSERT(pProcess);
			attachChild(pProcess);
		}
		else
		{
			GCC_ERROR("Attempting to attach child to ScriptProcess with no valid __object");
		}
	}
	else
	{
		GCC_ERROR("Invalid object type passed into ScriptProcess::ScriptAttachChild(); type = " + std::string(child.TypeName()));
	}
}

void ant::IScriptProcess::registerScriptClass( void )
{
	LuaPlus::LuaObject metaTableObj = LuaStateManager::instance()->getGlobalVars().CreateTable(SCRIPT_PROCESS_NAME);
	metaTableObj.SetObject("__index", metaTableObj);
	metaTableObj.SetObject("base", metaTableObj);  // base refers to the parent class; ie the metatable
	metaTableObj.SetBoolean("cpp", true);
	registerScriptClassFunctions(metaTableObj);
	metaTableObj.RegisterDirect("create", &IScriptProcess::createFromScript);
}

LuaPlus::LuaObject ant::IScriptProcess::createFromScript( LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject originalSubClass )
{
	// Note: The self parameter is not use in this function, but it allows us to be consistent when calling
	// Create().  The Lua version of this function needs self.
	GCC_LOG("Script", std::string("Creating instance of ") + SCRIPT_PROCESS_NAME);

	IScriptProcess* pObj = GCC_NEW IScriptProcess;

	pObj->m_self.AssignNewTable(LuaStateManager::instance()->getLuaState());
	if (pObj->buildCppDataFromScript(originalSubClass, constructionData))
	{
		LuaPlus::LuaObject metaTableObj = LuaStateManager::instance()->getGlobalVars().Lookup(SCRIPT_PROCESS_NAME);
		GCC_ASSERT(!metaTableObj.IsNil());

		pObj->m_self.SetLightUserData("__object", pObj);
		pObj->m_self.SetMetaTable(metaTableObj);
	}
	else
	{
		pObj->m_self.AssignNil(LuaStateManager::instance()->getLuaState());
		SAFE_DELETE(pObj);
	}

	return pObj->m_self;
}

void ant::IScriptProcess::registerScriptClassFunctions( LuaPlus::LuaObject& metaTableObj )
{
	metaTableObj.RegisterObjectDirect("succeedAndStop",    (IProcess*)0, &IProcess::succeedAndStop);
	metaTableObj.RegisterObjectDirect("failAndStop",       (IProcess*)0, &IProcess::failAndStop);
	metaTableObj.RegisterObjectDirect("pause",      (IProcess*)0, &IProcess::pause);
	metaTableObj.RegisterObjectDirect("unPause",    (IProcess*)0, &IProcess::unPause);
	metaTableObj.RegisterObjectDirect("isAlive",    (IScriptProcess*)0, &IScriptProcess::scriptIsAlive);
	metaTableObj.RegisterObjectDirect("isDead",     (IScriptProcess*)0, &IScriptProcess::scriptIsDead);
	metaTableObj.RegisterObjectDirect("isPaused",   (IScriptProcess*)0, &IScriptProcess::scriptIsPaused);
	metaTableObj.RegisterObjectDirect("attachChild",(IScriptProcess*)0, &IScriptProcess::scriptAttachChild);
}

