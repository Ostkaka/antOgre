#include <ant/resources/ResourceLoaders.hpp>
#include <ant/luascripting/LuaStateManager.hpp>

using namespace ant;


//////////////////////////////////////////////////////////////////////////
// DefaultResourceLoader
//////////////////////////////////////////////////////////////////////////

DefaultResourceLoader::DefaultResourceLoader()
{

}

DefaultResourceLoader::~DefaultResourceLoader()
{

}

std::string DefaultResourceLoader::getPattern()
{
	return "*";
}

bool DefaultResourceLoader::useRawFile()
{
	return true;
}

bool DefaultResourceLoader::discardRawBufferAfterLoad()
{
	return true;
}

ant::UInt DefaultResourceLoader::getLoadedResourceSize( char *rawBuffer, unsigned int rawSize )
{
	return rawSize;
}

bool DefaultResourceLoader::loadResource( char* rawBuffer, ant::UInt rawSize, ResourceHandleStrongPtr handle )
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ScriptResourceLoader
//////////////////////////////////////////////////////////////////////////
bool ant::ScriptResourceLoader::loadResource( char *rawBuffer, unsigned int rawSize, ResourceHandleStrongPtr handle )
{
	if (rawSize <= 0)
	{
		return false;
	}

	if (LuaStateManager::instance())
	{
		LuaStateManager::instance()->executeString(rawBuffer);
	}

	return true;
}

IResourceLoaderStrongPtr CreateScriptResourceLoader()
{
	return IResourceLoaderStrongPtr(GCC_NEW ScriptResourceLoader());
}