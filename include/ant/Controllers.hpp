#ifndef CONTROLLERS_HPP_
	#define CONTROLLERS_HPP_

#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>
#include <ant/ant_std.hpp>

namespace ant
{
	class IKeyboardController : public OIS::KeyListener 
	{
	public:
		virtual ~IKeyboardController() {}
		virtual bool keyPressed(const OIS::KeyEvent &arg) = 0;
		virtual bool keyReleased(const OIS::KeyEvent &arg) = 0;
	};

	class IMouseController : public OIS::MouseListener
	{
	public:
		virtual ~IMouseController() {}
		virtual bool mouseMoved(const OIS::MouseEvent &arg) = 0;
		virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
		virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
	};
}

#endif //CONTROLLERS_HPP_