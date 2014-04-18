#include <ant/actors/BaseScriptComponent.hpp>
#include <ant/actors/TransformComponent.hpp>
#include <ant/gccUtils/templates.hpp>
#include <ant/core_types.hpp>
#include <ant/gccUtils/String.hpp>
#include <ant/luascripting/LuaStateManager.hpp>

using namespace ant;

// This is the name of the metatable where all the function definitions exported to Lua will live.
static const char* BASESCRIPTCOMPONENT_METATABLE_NAME = "BaseScriptComponentMetaTable";
const char* BaseScriptComponent::g_Name = "BaseScriptComponent";

ant::BaseScriptComponent::BaseScriptComponent( void )
{
	m_scriptObject.AssignNil(LuaStateManager::instance()->getLuaState());
	m_scriptDestructorObject.AssignNil(LuaStateManager::instance()->getLuaState());
}

ant::BaseScriptComponent::~BaseScriptComponent( void )
{
	// call the script destructor
	if (m_scriptDestructorObject.IsFunction())
	{
		LuaPlus::LuaFunction<void> func(m_scriptDestructorObject);
		func(m_scriptObject);
	}

	// clear the script object
	m_scriptObject.AssignNil(LuaStateManager::instance()->getLuaState());

	// If we were given a path, remove it.
	if (!m_scriptObjectName.empty())
	{
		m_scriptObjectName += " = nil";
		LuaStateManager::instance()->executeString(m_scriptObjectName.c_str());
	}
}

bool ant::BaseScriptComponent::init( TiXmlElement* pData )
{
	LuaStateManager * stateMgr = LuaStateManager::instance();
	GCC_ASSERT(stateMgr);

	// Load the <ScriptObject> tag and validate it
	TiXmlElement* pScriptObjectElement = pData->FirstChildElement("ScriptObject");
	if (!pScriptObjectElement)
	{
		GCC_ERROR("No <ScriptObject> tag in XML.");
		return true;
	}

	// Read all the attributes
	const char* temp = NULL;
	temp = pScriptObjectElement->Attribute("var");
	if (temp)
	{
		m_scriptObjectName = temp;
	}

	temp = pScriptObjectElement->Attribute("constructor");
	if (temp)
	{
		m_constructorName = temp;
	}

	temp = pScriptObjectElement->Attribute("destructor");
	if (temp)
	{
		m_destructorName = temp;
	}

	// If we have var attribute, export the instance of the object to the lua context
	if (!m_scriptObjectName.empty())
	{
		m_scriptObject = stateMgr->createPath(m_scriptObjectName.c_str());

		if (!m_scriptObject.IsNil())
		{
			createScriptObject();
		}
	}

	// The scriptConstructor attribute will also cause a lua object ot be created if one was
	// not created in the last step. ScriptConstructor string is then used as a function for 
	// creating the object.
	if (!m_constructorName.empty())
	{
		m_scriptConstructorObject = stateMgr->getGlobalVars().Lookup(m_constructorName.c_str());
		if (m_scriptConstructorObject.IsFunction())
		{
			// If the script object is nil, a lua object is create here instead with the constructor function
			if (m_scriptObject.IsNil())
			{
				m_scriptObject.AssignNewTable(stateMgr->getLuaState());
				createScriptObject();
			}
		}
	}

	// The scriptDestructor attribute is treated as a function in the form of f(scriptObject) and is called
	// when the C++ ScriptObject instance is destroyed.
	if (!m_destructorName.empty())
	{
		m_scriptDestructorObject = stateMgr->getGlobalVars().Lookup(m_destructorName.c_str());
	}

	// Read the <ScriptData> tag
	TiXmlElement* pDataElement = pData->FirstChildElement("ScriptData");
	if (pDataElement)
	{
		if (m_scriptObject.IsNil())
		{
			GCC_ERROR("m_scriptObject cannot be nil when ScriptData is defined");
		}

		for (TiXmlAttribute* pAttribute = pDataElement->FirstAttribute(); pAttribute != NULL; pAttribute = pAttribute->Next())
		{
			m_scriptObject.SetString(pAttribute->Name(), pAttribute->Value());
		}
	}

	return true;
}

void ant::BaseScriptComponent::postInit( void )
{
	// call the script constructor
	if (m_scriptConstructorObject.IsFunction())
	{
		LuaPlus::LuaFunction<void> func(m_scriptConstructorObject);
		func(m_scriptObject);
	}
}

TiXmlElement* ant::BaseScriptComponent::generateXml( void )
{
	TiXmlElement* pBaseElement = GCC_NEW TiXmlElement(getName());

	TiXmlElement* pScriptObjectElement = GCC_NEW TiXmlElement("ScriptObject");
	if (!m_scriptObjectName.empty())
		pScriptObjectElement->SetAttribute("var", m_scriptObjectName.c_str());
	if (!m_constructorName.empty())
		pScriptObjectElement->SetAttribute("constructor", m_constructorName.c_str());
	if (!m_destructorName.empty())
		pScriptObjectElement->SetAttribute("destructor", m_destructorName.c_str());
	pBaseElement->LinkEndChild(pScriptObjectElement);

	return pBaseElement;
}

void ant::BaseScriptComponent::createScriptObject( void )
{
	LuaStateManager* pStateMgr = LuaStateManager::instance();
	GCC_ASSERT(pStateMgr);
	GCC_ASSERT(!m_scriptObject.IsNil());

	LuaPlus::LuaObject metaTableObj = pStateMgr->getGlobalVars().Lookup(BASESCRIPTCOMPONENT_METATABLE_NAME);
	GCC_ASSERT(!metaTableObj.IsNil());

	LuaPlus::LuaObject boxedPtr = pStateMgr->getLuaState()->BoxPointer(this);
	boxedPtr.SetMetaTable(metaTableObj);
	m_scriptObject.SetLightUserData("__object", this);
	m_scriptObject.SetMetaTable(metaTableObj);
}

LuaPlus::LuaObject ant::BaseScriptComponent::getActorId( void )
{
	LuaPlus::LuaObject ret;
	ret.AssignInteger(LuaStateManager::instance()->getLuaState(),m_pOwner->getId());
	return ret;
}

void ant::BaseScriptComponent::registerScriptFunctions( void )
{
	// Create the metatable
	LuaPlus::LuaObject metaTableObj = LuaStateManager::instance()->getGlobalVars().CreateTable(BASESCRIPTCOMPONENT_METATABLE_NAME);
	metaTableObj.SetObject("__index", metaTableObj);

	// Do the rest of the registering of the Lua export functions here
	metaTableObj.RegisterObjectDirect("getActorId",		        (BaseScriptComponent*)0, &BaseScriptComponent::getActorId);
	metaTableObj.RegisterObjectDirect("getPos",                 (BaseScriptComponent*)0, &BaseScriptComponent::getPos);
}

void ant::BaseScriptComponent::unregisterScriptFunctions( void )
{
	LuaPlus::LuaObject metaTableObj = LuaStateManager::instance()->getGlobalVars().Lookup(BASESCRIPTCOMPONENT_METATABLE_NAME);
	if (!metaTableObj.IsNil())
		metaTableObj.AssignNil(LuaStateManager::instance()->getLuaState());
}

LuaPlus::LuaObject ant::BaseScriptComponent::getPos(void)
{
	LuaPlus::LuaObject ret;

	shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->getComponent<TransformComponent>(TransformComponent::g_Name));
	if (pTransformComponent)
		LuaStateManager::instance()->convertVec2ToTable(pTransformComponent->getPostion(), ret);
	else
		ret.AssignNil(LuaStateManager::instance()->getLuaState());

	return ret;
}
