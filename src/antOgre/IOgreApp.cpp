#include <antOgre/IOgreApp.hpp>
#include <ant/BaseGameLogic.hpp>
#include <ant/IEventManager.hpp>
#include <ant/Events.hpp>
#include <ant/ResourceCacheManager.hpp>
#include <ant/ResourceCache.hpp>
#include <ant/ResourceFiles.hpp>
#include <ant/TextureResource.hpp>
#include <ant/Resource.hpp>
#include <antLua/LuaStateManager.hpp>
#include <antLua/ScriptProcess.hpp>
#include <antLua/LuaScriptExports.hpp>
#include <antLua/BaseScriptComponent.hpp>
#include <ant/GenericObjectFactory.hpp>
#include <OGRE/OgreTimer.h>

using namespace ant;

const char* SCRIPT_PREINIT_FILE = "lua\\PreInit.lua";

/// Single instance of the most recently created IOgreApp class
IOgreApp* IOgreApp::g_App = NULL;

ant::IOgreApp::IOgreApp(const std::string theTitle /*= "Ant Application"*/)
: m_gameLogic(nullptr),
m_eventManager(nullptr),
m_renderWindow(nullptr),
m_ogreRoot(nullptr),
m_pInputMgr(nullptr),
m_running(false),
m_updateRate(0.016),
m_initialized(false),
m_title(theTitle)
{
	g_App = this;
}

ant::IOgreApp::~IOgreApp()
{

}

void ant::IOgreApp::processArguments(int argc, char* argv[])
{
	// TODO
}

bool ant::IOgreApp::initResourceCache()
{
	bool resCacheInitialized = false;

	// Init the resource cache in the resource handler singleton
	std::string zipPath = ANT_DATA_PATH"\\unittestdata\\zipdata.zip";
	IResourceFile * resFile = ANT_NEW DevelopmentResourceZipFile(L"Assets.zip", DevelopmentResourceZipFile::Editor);

	ResourceCacheManager::startUp();
	resCacheInitialized = ResourceCacheManager::instance()->initResourceCache(50, resFile);

	// Declare and find the resource loaders
	extern IResourceLoaderStrongPtr CreateXmlResourceLoader();
	extern IResourceLoaderStrongPtr CreateScriptResourceLoader();
	extern IResourceLoaderStrongPtr CreatePNGResourceLoader();
	extern IResourceLoaderStrongPtr CreateJPGResourceLoader();

	// Register the loaders 
	ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreateXmlResourceLoader());
	ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreateScriptResourceLoader());
	ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreatePNGResourceLoader());
	ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreateJPGResourceLoader());

	return resCacheInitialized;
}

bool ant::IOgreApp::init()
{
	// Init Logger
	Logger::Init(ANT_DATA_PATH"\\default_logging.xml");

	ANT_LOG("OgreApplication", "Init application");

	// Init resource cache
	if (!initResourceCache())
	{
		ANT_ERROR("Failed to initialize resource cache");
	}

	// init Lua state manager
	if (!initScriptEngine())
	{
		ANT_ERROR("Failed to initialize LuaStateManager");
	}

	// init the event system
	if (!initEventSystem())
	{
		ANT_ERROR("Failed to create EventManager.");
	}

	// Register events
	registerEngineEvents();
	registerGameEvents();

	// Init renderer and application window
	initRenderer();

	// Init the game logic and the views from a virtual function create by child class
	m_gameLogic = initGameLogicAndView();
	if (!m_gameLogic)
	{
		return false;
	}

	m_initialized = true;

	return true;
}

int ant::IOgreApp::run()
{
	ANT_LOG("OgreApplication", "Running application");
	ANT_ASSERT(m_initialized);

	// Init the application
	m_running = true;

	// Start the main loop of the application
	gameLoop();

	// Clean up all the stuff in the application
	cleanup();

	m_running = false;

	return 1;
}

void ant::IOgreApp::gameLoop()
{
	ANT_LOG("OgreApplication", "Starting gameloop");

	// Init the update clock
	Ogre::Timer updateClock;

	// Init the frame clock
	Ogre::Timer frameClock;

	// Start up the clock
	updateClock.reset();

	// When do we update next?
	int updateNext = updateClock.getMilliseconds();

	// Begin the main loop
	while (isRunning())
	{
		// Get the current time
		ant::DeltaTime updateTime = ant::Real(updateClock.getMilliseconds() * 1000.0);

		// Is it time to update yet?
		ant::DeltaTime dt = ant::Real(frameClock.getMilliseconds() * 1000.0);
		if (dt > m_updateRate)
		{
			// Process input and get it to the game logic and it's views
			processInput();

			// Process events
			IEventManager::instance()->update(); // allow event queue to process for up to 20 ms

			// Update game logic
			m_gameLogic->onUpdate(updateTime, dt);

			// Update renderer
			renderFrame(updateTime, dt);

			// Restart frame clock
			frameClock.reset();
		}
	}
}

bool ant::IOgreApp::isRunning() const
{
	return m_running;
}

ant::Real ant::IOgreApp::getUpdateRate() const
{
	return m_updateRate;
}

void ant::IOgreApp::setUpdateRate(float rate)
{
	m_updateRate = rate;
}

void ant::IOgreApp::quit(int exitCode /*= 1*/)
{
	m_exitCode = exitCode;
	m_running = false;
}

void ant::IOgreApp::initRenderer()
{
	ANT_LOG("IOgreApp", "Init Renderer");

	m_ogreRoot = new Ogre::Root();

	if (!m_ogreRoot->showConfigDialog())
		ANT_ERROR("Could not show ogre config dialog!");

	m_renderWindow = m_ogreRoot->initialise(true, m_title);

	m_viewport = m_renderWindow->addViewport(0);
	m_viewport->setBackgroundColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f, 1.0f));

	m_viewport->setCamera(0);

	m_renderWindow->setActive(true);
}

void ant::IOgreApp::processInput()
{
	// Capture events here!
		
}

void IOgreApp::initInputSystem()
{
	ANT_ASSERT(m_renderWindow);
	size_t hWnd = 0;
	OIS::ParamList paramList;
	m_renderWindow->getCustomAttribute("WINDOW", &hWnd);

	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

}

void ant::IOgreApp::renderFrame(ant::DeltaTime fTime, ant::DeltaTime dt)
{
	if (m_gameLogic)
	{
		for (GameViewList::iterator it = m_gameLogic->getGameViews()->begin(),
			end = m_gameLogic->getGameViews()->end(); it != end; ++it)
		{
			(*it)->onRender(fTime, dt);
		}
	}

	// Render once !
	m_ogreRoot->renderOneFrame();

	// TODO - g_pApp->m_pGame->VRenderDiagnostics();
}

bool ant::IOgreApp::initScriptEngine()
{
	bool luaInitialized = false;

	// First, create the Lua state manager
	luaInitialized = LuaStateManager::startUp();

	// Load the pre-init file
	{
		Resource resource(SCRIPT_PREINIT_FILE);
		ResourceHandleStrongPtr pPreInitResource = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);
	}

	// Register all functions exported from c++
	LuaScriptExports::registerScripts();
	IScriptProcess::registerScriptClass();
	BaseScriptComponent::registerScriptFunctions();

	return luaInitialized;
}

void ant::IOgreApp::cleanup()
{
	// Clean up the following

	// Resource manager
	ResourceCacheManager::shutDown();

	// Lua
	LuaScriptExports::unregisterScripts();
	BaseScriptComponent::unregisterScriptFunctions();
}

bool ant::IOgreApp::initEventSystem()
{
	m_eventManager = ANT_NEW EventManager("SFMLApp Event Mgr", true);
	return m_eventManager != NULL;
}

void ant::IOgreApp::registerEngineEvents(void)
{
	REGISTER_EVENT(EvtData_New_Actor);
	REGISTER_EVENT(EvtData_Destroy_Actor);
}

IOgreApp * ant::IOgreApp::getApp()
{
	return g_App;
}

IGameLogic * ant::IOgreApp::getGameLogic()
{
	return m_gameLogic;
}
