#ifndef  IMPUTMANAGER_HPP_
	#define IMPUTMANAGER_HPP_

#include <ant/ant_std.hpp>
#include <OIS\OISInputManager.h>
#include <OIS\OISKeyboard.h>
#include <OIS\OISMouse.h>

namespace ant
{
	/**
	* Singleton manager for handling input through the IOS system
	*/
	class InputManager
	{
		typedef std::vector<OIS::Object*> InputObjectVector;

	public:
		InputManager();

		static void startUp();

		static void shutDown();		

		void captureInput();

		void AddListener(OIS::KeyListener*   listener);

		void AddListener(OIS::MouseListener* listener);

		static InputManager* instance();

	protected:

		void init();

		~InputManager();

		static InputManager*        g_inputMgr;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		// OIS Input Manager
		OIS::InputManager*			m_inputMgr;
		OIS::Keyboard*              m_keyboard;
		OIS::Mouse*                 m_mouse;

		InputObjectVector           m_objects;
	};

	ANT_INLINE InputManager* InputManager::instance() { ANT_ASSERT(g_inputMgr); return g_inputMgr; }

}

#endif // IMPUTMANAGER_HPP_