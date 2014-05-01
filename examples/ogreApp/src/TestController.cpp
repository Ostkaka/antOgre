#include "TestController.hpp";
#include <ant\InputManager.hpp>
#include <antOgre\IOgreApp.hpp>
#include <ant\templates.hpp>
#include <actors\TransformComponent.hpp>

ant::Real MOVE_INC = 5.0;

ant::TestController::TestController(Ogre::SceneNode * target)
{
	m_actorId = INVALID_ACTOR_ID;
	m_target  = target;

	init();
}

ant::TestController::TestController(ActorId target)
{
	m_actorId = target;
	m_target  = nullptr;

	init();
}

ant::TestController::~TestController()
{
	//ant::InputManager::instance()->(this);
}

void ant::TestController::init()
{
	moveForward = false;
	moveLeft    = false;
	moveRight   = false;
	moveBack    = false;

	ant::InputManager::instance()->AddCallback(this);
}

bool ant::TestController::keyPressed(OIS::KeyEvent const& arg)
{
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
	case OIS::KeyCode::KC_ESCAPE:
		IOgreApp::getApp()->quit();
		break;
	default:
		break;
	}
		
	return true;
}

bool ant::TestController::keyReleased(OIS::KeyEvent const& arg)
{
	// Do nothing
	switch (arg.key)
	{
	case OIS::KeyCode::KC_W:
		moveForward = false;
		break;
	case OIS::KeyCode::KC_D:
		moveRight = false;
		break;
	case OIS::KeyCode::KC_A:
		moveLeft = false;
		break;
	case OIS::KeyCode::KC_S:
		moveBack = false;
		break;
	default:
		break;
	}

	return true;
}

void ant::TestController::update()
{
	Ogre::Vector3 inc(moveLeft ? -MOVE_INC : (moveRight ? MOVE_INC : 0), 0, moveForward ? -MOVE_INC : (moveBack ? MOVE_INC : 0));

	if (m_actorId == INVALID_ACTOR_ID && m_target != nullptr)
	{
		Ogre::Vector3 pos = m_target->getPosition();
		m_target->setPosition(pos + inc);
	}
	else if (m_actorId != INVALID_ACTOR_ID)
	{
		// Get actor transform component
		ActorStrongPtr str = MakeStrongPtr(IOgreApp::getApp()->getGameLogic()->getActor(m_actorId));
		TransformComponentStrongPtr p = MakeStrongPtr(str->getComponent<TransformComponent>(TransformComponent::g_Name));
		if (p)
		{
			Vec3 pos = p->getPosition() + OGRE_VEC3_TO_ANT_VEC3(inc);
			p->setPosition(pos);
		}
	}
}