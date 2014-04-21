#include <ant/TextureResource.hpp>
#include <ant/IResourceLoader.hpp>
#include <ant\ResourceHandle.hpp>

using namespace ant;

ant::UInt ant::SFMLTextureResourceLoader::getLoadedResourceSize( char* rawBuffer, ant::UInt rawSize )
{
	return rawSize;
}

bool ant::SFMLTextureResourceLoader::loadResource( char *rawBuffer, unsigned int rawSize, ResourceHandleStrongPtr handle )
{
	// Cheat here and use sf::Texture to load it from memory first to see if it works
	return false;
}

IResourceLoaderStrongPtr CreatePNGResourceLoader()
{
	return IResourceLoaderStrongPtr(ANT_NEW PngResourceLoader());
}

IResourceLoaderStrongPtr CreateJPGResourceLoader()
{
	return IResourceLoaderStrongPtr(ANT_NEW JpgResourceLoader());
}