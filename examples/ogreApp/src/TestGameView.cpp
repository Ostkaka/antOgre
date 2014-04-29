#include "TestGameView.hpp"
#include "TestEvents.hpp"
#include <antOgre/OGREHumanView.hpp>
#include <ant/InputManager.hpp>

using namespace ant;

antOgre::TestGameView::TestGameView() : antOgre::OGREHumanView()
{
	registerAllDelegates();
}

antOgre::TestGameView::~TestGameView()
{
	removeAllDelegates();
}

void antOgre::TestGameView::setControllerActor(ActorId actorId)
{
	// Try to find node for actor
	Ogre::SceneNode * node = m_scene->findActor(actorId);

	if (!node)
	{
		ANT_WARNING("Could not find node for actor!");
	}

	OGREHumanView::setControllerActor(actorId);

	m_controller.reset(ANT_NEW TestController(actorId));
	
	//m_scene->getSceneManager()->getRootSceneNode()->removeChild(m_cameraNode);
	//node->addChild(m_cameraNode);
	m_cameraNode->detachObject(m_camera);
	node->attachObject(m_camera);
	m_camera->setPosition(0, 0, 100);
}

void antOgre::TestGameView::onAttach(GameViewId id, ActorId actorid)
{
	OGREHumanView::onAttach(id, actorid);
}

void antOgre::TestGameView::onUpdate(DeltaTime dt)
{
	// Update controllers here. This is probably also where we should RESPOND to captured input
	//std::cout << "Camera pos: " << m_cameraNode->getPosition() << std::endl;
}

void antOgre::TestGameView::setControllerActorDelegate(IEventDataStrongPtr eventPtr)
{
	shared_ptr<EvtData_Set_Controlled_Actor> pCastEventData = static_pointer_cast<EvtData_Set_Controlled_Actor>(eventPtr);
	setControllerActor(pCastEventData->getActorId());
}

bool antOgre::TestGameView::loadGameDelegate(TiXmlElement* levelData)
{
	if (!OGREHumanView::loadGameDelegate(levelData))
	{
		return false;
	}

	m_controller.reset(ANT_NEW TestController(m_cameraNode));

	return true;
}

void antOgre::TestGameView::renderText()
{
	// We should render the gui here
}

void antOgre::TestGameView::registerAllDelegates()
{
	IEventManager* pGlobalEventManager = IEventManager::instance();
	pGlobalEventManager->addListener(MakeDelegate(this, &TestGameView::setControllerActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}

void antOgre::TestGameView::removeAllDelegates()
{
	IEventManager* pGlobalEventManager = IEventManager::instance();
	pGlobalEventManager->removeListener(MakeDelegate(this, &TestGameView::setControllerActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}