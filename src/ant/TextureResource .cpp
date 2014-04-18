#include <ant/resources/TextureResource.hpp>
#include <ant/resources/IResourceLoader.hpp>
#include <SFML/Graphics.hpp>

using namespace ant;

ant::UInt ant::SFMLTextureResourceLoader::getLoadedResourceSize( char* rawBuffer, ant::UInt rawSize )
{
	return rawSize;
}

bool ant::SFMLTextureResourceLoader::loadResource( char *rawBuffer, unsigned int rawSize, ResourceHandleStrongPtr handle )
{
	// Cheat here and use sf::Texture to load it from memory first to see if it works
	sf::Texture tex;
	bool loadedTexture=false;
	return (tex.loadFromMemory(rawBuffer,rawSize));
}

IResourceLoaderStrongPtr CreatePNGResourceLoader()
{
	return IResourceLoaderStrongPtr(GCC_NEW PngResourceLoader());
}

IResourceLoaderStrongPtr CreateJPGResourceLoader()
{
	return IResourceLoaderStrongPtr(GCC_NEW JpgResourceLoader());
}