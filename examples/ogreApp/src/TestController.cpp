#include "TestController.hpp";
#include <ant\InputManager.hpp>

ant::TestController::TestController(Ogre::SceneNode * target)
{
	m_actorId = INVALID_ACTOR_ID;
	m_target  = target;
}

ant::TestController::TestController(ActorId target)
{
	m_actorId = target;
	m_target  = nullptr;

	// Hook this up to the input systemm

	ant::InputManager::instance()->AddListener(this);
}

ant::TestController::~TestController()
{
	//ant::InputManager::instance()->(this);
}

bool ant::TestController::keyPressed(OIS::KeyEvent const& arg)
{
	bool moveForward = false;
	bool moveLeft = false;
	bool moveRight = false;
	bool moveBack = false;

	switch (arg.key)
	{
	case OIS::KeyCode::KC_W:
		moveForward = true;
		break;
	case OIS::KeyCode::KC_D:
		moveRight = true;
		break;
	case OIS::KeyCode::KC_A:
		moveLeft = true;
		break;
	case OIS::KeyCode::KC_S:
		moveBack = true;
		break;
	default:
		break;
	}

	if (m_target != nullptr)
	{
		Ogre::Vector3 inc(moveLeft ? 10 : (moveRight ? -10 : 0), 0, moveForward ? 10 : (moveBack ? -10 : 0));

		Ogre::Vector3 pos = m_target->getPosition();
		m_target->setPosition(pos + inc);			;
	}

	return true;
}

bool ant::TestController::keyReleased(OIS::KeyEvent const& arg)
{
	// Do nothing
	return true;
}