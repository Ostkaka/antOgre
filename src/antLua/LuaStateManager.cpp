#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/gccUtils/String.hpp>

using namespace ant;

LuaStateManager* LuaStateManager::s_pInstance = NULL;

bool ant::LuaStateManager::create( void )
{
	if (s_pInstance)
	{
		GCC_ERROR("Overwriting LuaStateManager singleton");
		SAFE_DELETE(s_pInstance);
	}

	s_pInstance = GCC_NEW LuaStateManager;
	if (s_pInstance)
		return s_pInstance->init();

	return false;
}

void ant::LuaStateManager::destroy( void )
{
	GCC_ASSERT(s_pInstance);
	SAFE_DELETE(s_pInstance);
}

bool ant::LuaStateManager::init( void ) 
{
	m_pLuaState = LuaPlus::LuaState::Create(true);
	if (m_pLuaState == NULL)
		return false;

	// register functions
	m_pLuaState->GetGlobals().RegisterDirect("executeFile", (*this), &LuaStateManager::executeFile);
	m_pLuaState->GetGlobals().RegisterDirect("executeString", (*this), &LuaStateManager::executeString);

	return true;
}

void ant::LuaStateManager::executeFile( const char* path ) 
{
	int result = m_pLuaState->DoFile(path);
	if (result != 0)
	{
		setError(result);
	}
}

void ant::LuaStateManager::executeString( const char* str ) 
{
	int result = 0;

	if (strlen(str) <= 1 || str[0] != '=')
	{
		result = m_pLuaState->DoString(str);
		if (result != 0)
		{
			setError(result);
		}
	}

	// If the statement starts with '=', wrap the statement in the print() function
	else
	{
		std::string buffer("print(");
		buffer += (str + 1);
		buffer += ")";
		result = m_pLuaState->DoString(buffer.c_str());
		if (result != 0)
			setError(result);
	}
}

ant::LuaStateManager::LuaStateManager( void )
{
	m_pLuaState = NULL;
}

ant::LuaStateManager::~LuaStateManager( void )
{
	if (m_pLuaState)
	{
		LuaPlus::LuaState::Destroy(m_pLuaState);
		m_pLuaState = NULL;
	}
}

LuaPlus::LuaObject ant::LuaStateManager::createPath( const std::string& path, bool ignoreLastElement /*= false*/ )
{
	StringVec splitPath;
	Split(path,splitPath, '.');
	if (ignoreLastElement)
	{
		splitPath.pop_back();
	}

	LuaPlus::LuaObject context = getGlobalVars();
	for (auto it = splitPath.begin() ; it != splitPath.end() ; it++)
	{
		// Is the context valid?
		if (context.IsNil())
		{
			GCC_ERROR("Something broke in CreatePath(); bailing out (element == " + (*it) + ")");
			return context;  // this will be nil
		}

		// grab whatever exists for this element
		const std::string& element = (*it);
		LuaPlus::LuaObject curr = context.GetByName(element.c_str());

		if (!curr.IsTable())
		{
			// if the element is not a table and not nil, we clobber it
			if (!curr.IsNil())
			{
				GCC_WARNING("Overwriting element '" + element + "' in table");
				context.SetNil(element.c_str());
			}

			// element is either nil or was clobbered so add the new table
			context.CreateTable(element.c_str());
		}

		context = context.GetByName(element.c_str());
	}

	// We have created a complete path here
	return context;
}

void ant::LuaStateManager::setError( int errorNum )
{
	// Note: If we get an error, we're hosed because LuaPlus throws an exception.  So if this function
	// is called and the error at the bottom triggers, you might as well pack it in.

	LuaPlus::LuaStackObject stackObj(m_pLuaState,-1);
	const char* errStr = stackObj.GetString();
	if (errStr)
	{
		m_lastError = errStr;
		clearStack();
	}
	else
	{
		m_lastError = "Unkown Lua parse error";
	}

	GCC_ERROR(m_lastError);
}

void ant::LuaStateManager::clearStack( void )
{
	m_pLuaState->SetTop(0);
}

LuaPlus::LuaObject ant::LuaStateManager::getGlobalVars( void )
{
	GCC_ASSERT(m_pLuaState);
	return m_pLuaState->GetGlobals();
}

LuaPlus::LuaState* ant::LuaStateManager::getLuaState( void ) const
{
	return m_pLuaState;
}


void ant::LuaStateManager::convertVec2ToTable(const sf::Vector2f& vec, LuaPlus::LuaObject& vec2Table)
{
	vec2Table.AssignNewTable(getLuaState());
	vec2Table.SetNumber("x", vec.x);
	vec2Table.SetNumber("y", vec.y);
}
