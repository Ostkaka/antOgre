#ifndef IOGREAPP_HPP_
	#define IOGREAPP_HPP_

#include <fstream>
#include <stddef.h>
#include <string.h>
#include <vector>
#include <ant/core_types.hpp>
#include <ant_config.hpp>
#include <ant\BaseGameLogic.hpp>
#include <OIS\OISInputManager.h>
#include <OGRE\OgreRenderWindow.h>
#include <OGRE\OgreRoot.h>

namespace ant
{
	struct GameSettings
	{
		GameSettings(){ m_level = ""; }
		~GameSettings(){}

		std::string m_level;
	};

	/**
	* Base app class for ogre based application
	*/
	class IOgreApp
	{
	public:
		/**
		* IApp deconstructor
		*/
		virtual ~IOgreApp();

		/**
		* Will return the most recent created IApp based class. Used for retrieving
		* assets among other things
		*/
		static  IOgreApp* getApp();

		/*
		* Processes the command line for arguments
		*/
		virtual void processArguments(int argc, char* argv[]);

		/**
		* Inits the app instance
		*/
		bool init();

		/**
		* Valued of the exit-code given by the application by using quit.
		*/
		int run();

		/**
		* Used to check in case the application is still running
		*/
		bool isRunning() const;

		/**
		* Returns the current game loop update rate being
		* used in Hz (updates per second)
		*/
		ant::Real getUpdateRate() const;

		/**
		* SetUpdateRate will set the game loop update to rate
		*/
		void setUpdateRate(float rate);

		/**
		* Quit will signal the Application to stop running.
		*/
		void quit(int exitCode = 1);

		/**
		* Get the game logic from the APP
		*/
		IGameLogic * getGameLogic();

	protected:

		/**
		* Basic constructor
		*/
		IOgreApp(const std::string theTitle = "Ant Application");

		/**
		* Responsible for monitoring IsRunning and exiting when the
		* Application is done.
		*/
		virtual void gameLoop();

		/**
		* Registers events for the game
		*/
		virtual void registerGameEvents(void){}

		/**
		* ProcessInput is responsible for performing all input processing for
		* the game loop.
		*/
		virtual void processInput();

		/**
		* Initializes the game logic and views.
		*/
		virtual BaseGameLogic* initGameLogicAndView() = 0;

	private:

		/**
		* Initializes the resource global resource cache
		*/
		bool initResourceCache();

		/**
		* Initializes the script engine
		*/
		bool initScriptEngine();

		/**
		* Initializes the event system
		*/
		bool initEventSystem();

		/**
		* Registers engine specific events that consists of the backbone
		*/
		void registerEngineEvents(void);

		/**
		* Initializes the Rendering window that
		* will be used to display the games graphics.
		*/
		void initRenderer();

		/**
		* Inits the input system
		*/
		void initInputSystem();

		/**
		* Renders the frame for the game, given that human views are connected to it
		*/
		void renderFrame(ant::DeltaTime fTime, ant::DeltaTime dt);

		/**
		* Responsible for registering and loading the
		* application configuration file (settings.cfg)
		*/
		void initConfig();

		/**
		* Cleanup is responsible for performing any last minute Application
		* cleanup steps before exiting the Application.
		*/
		void cleanup();

		/**
		* App copy constructor is private because we do not allow copies of
		* singleton class. Intentionally undefined.
		*/
		IOgreApp(const IOgreApp&);

		/**
		* Assignment operator is private because copies
		* of our Singleton class are not allowed. Intentionally undefined.
		*/
		IOgreApp& operator=(const IOgreApp&);

	public:
		///CONSTANTS
		/// Default Video width
		static const unsigned int DEFAULT_VIDEO_WIDTH = 800;
		/// Default Video height 
		static const unsigned int DEFAULT_VIDEO_HEIGHT = 600;
		/// Default video bits per pixel (color depth) 
		static const unsigned int DEFAULT_VIDEO_BPP = 32;
		/// Default application wide settings file string
		static const char* APP_SETTINGS;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
		/// Title to use for Window
		std::string                  m_title;

		/// Settings for the application	
		GameSettings                 m_settings;

		/// The game logic
		BaseGameLogic*			     m_gameLogic;

		// EventManager
		EventManager*				 m_eventManager;

		// OIS Input Manager
		OIS::InputManager*			m_pInputMgr;

		// Ogre stuff
		Ogre::Root*					m_ogreRoot;
		Ogre::RenderWindow*			m_renderWindow;
		Ogre::Viewport*				m_viewport;

	private:
		/// Static instance variables assigned at creation
		static IOgreApp*        g_App;

		/// Exit code that is returned upon existing the application
		int						m_exitCode;

		/// TRUE if the app is still running
		bool					m_running;

		/// True if the application is initialized
		bool					m_initialized;

		/// Value that holds the update rate in seconds used for a fixed loop time
		ant::DeltaTime m_updateRate;
	};
}




#endif /*IOGREAPP_HPP_*/