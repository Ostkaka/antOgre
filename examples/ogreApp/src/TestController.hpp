#ifndef TESTCONTROLLER_HPP_
	#define TESTCONTROLLER_HPP_

#include <ant/Controllers.hpp>
#include <ant/ant_std.hpp>
#include <OGRE\OgreSceneNode.h>
#include <actors\Actor.hpp>

namespace ant
{
	ANT_DECLARE_POINTER_TYPES(TestController)

	class TestController : public IKeyboardController
	{
	public:

		TestController(Ogre::SceneNode * target);

		TestController(ActorId target);

		virtual ~TestController();

		virtual bool keyPressed(const OIS::KeyEvent &arg) ANT_OVERRIDE;

		virtual bool keyReleased(const OIS::KeyEvent &arg) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		Ogre::SceneNode * m_target;
		ActorId           m_actorId;
	};
}


#endif // TESTCONTROLLER_HPP_