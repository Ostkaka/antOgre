#include <antOgre\RenderManager.hpp>

using namespace antOgre; 
using namespace ant;

RenderManager* RenderManager::g_renderManager = nullptr;

RenderManager::RenderManager() : 
m_renderWindow(nullptr),
m_ogreRoot(nullptr)
{
}

RenderManager::~RenderManager()
{
	// Delete the ogre root
	SAFE_DELETE(m_ogreRoot);
}

Ogre::SceneManager* RenderManager::getOrCreateSceneManager(const std::string& name)
{
	ANT_ASSERT(m_ogreRoot);

	if (m_ogreRoot->hasSceneManager(name))
	{
		return m_ogreRoot->getSceneManager(name);
	}
	else
	{
		return m_ogreRoot->createSceneManager(name);
	}
}

void RenderManager::renderFrame()
{
	ANT_ASSERT(m_ogreRoot);

	m_ogreRoot->renderOneFrame();
}

void RenderManager::init()
{
	std::string plugins(ANT_DATA_PATH"/plugins.cfg");

	m_ogreRoot = new Ogre::Root(plugins);

	//if (!m_ogreRoot->showConfigDialog())
	//	ANT_ERROR("Could not show ogre config dialog!");

	Ogre::RenderSystem *rs = m_ogreRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

	// or use "OpenGL Rendering Subsystem"
	m_ogreRoot->setRenderSystem(rs);
}

void RenderManager::startUp()
{
	ANT_ASSERT(g_renderManager == nullptr);

	g_renderManager = ANT_NEW RenderManager();

	g_renderManager->init();
}

void RenderManager::shutDown()
{
	SAFE_DELETE(g_renderManager);
}

Ogre::RenderWindow* RenderManager::createRenderWindow(const std::string& title)
{
	ANT_ASSERT(m_ogreRoot);

	m_renderWindow = m_ogreRoot->initialise(true, title);

	// Get the SceneManager, in this case a generic one
	m_ogreRoot->createSceneManager(Ogre::ST_GENERIC, "main");

	// Create one viewport, entire window // TODO - viewports and cameras should be created in the HumanView 
	Ogre::Camera* camera = m_ogreRoot->getSceneManager("main")->createCamera("MainCamera");

	camera->setPosition(0, 0, 150);
	camera->lookAt(0, 0, 0);

	ANT_ASSERT(m_renderWindow);
	return m_renderWindow;
}