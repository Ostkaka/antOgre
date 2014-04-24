#ifndef RENDERMANAGER_HPP_
#define RENDERMANAGER_HPP

#include <ant/ant_std.hpp>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderWindow.h>

namespace antOgre
{
	/**
	* Singleton that handles the rendering of the game
	*/
	class RenderManager
	{
	public:
		static void startUp();
		
		static void shutDown();

		static RenderManager* instance();

		Ogre::RenderWindow* createRenderWindow(const std::string& title);

		Ogre::RenderWindow* getRenderWindow();

		Ogre::SceneManager* getOrCreateSceneManager(const std::string& name);

		void renderFrame();

		// TODO - Add functions that can draw lines and spheres and stuff

	protected:
		
		void init();

	protected:
		RenderManager();
		~RenderManager();
		RenderManager(const RenderManager& x);
		RenderManager& operator=(const RenderManager& x);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		// Ogre stuff
		Ogre::Root*					m_ogreRoot;
		Ogre::RenderWindow*			m_renderWindow;
		Ogre::Viewport*				m_viewport;

		static RenderManager*       g_renderManager;
	};

	ANT_INLINE RenderManager* RenderManager::instance()
	{
		ANT_ASSERT(g_renderManager);
		return g_renderManager;
	}

	ANT_INLINE Ogre::RenderWindow* RenderManager::getRenderWindow()
	{
		ANT_ASSERT(m_renderWindow);
		return m_renderWindow;
	}
}



#endif // RENDERMANAGER_HPP_