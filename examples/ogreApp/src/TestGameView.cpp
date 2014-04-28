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

	m_controller.reset(ANT_NEW TestController(node));
}

void antOgre::TestGameView::onAttach(GameViewId id, ActorId actorid)
{
	OGREHumanView::onAttach(id, actorid);
}

void antOgre::TestGameView::onUpdate(DeltaTime dt)
{
	// Update controllers here. This is probably also where we should capture input!!
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