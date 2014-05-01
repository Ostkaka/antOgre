#include <antOgre/OgreScene.hpp>
#include <ant\Events.hpp>
#include <antOgre\IOgreApp.hpp>
#include <ant\templates.hpp>
#include <actors\TransformComponent.hpp>

using namespace antOgre;

OgreScene::OgreScene(const std::string& sceneName)
{
	// Create a scene manager in the current Render Manager
	m_sceneMgr = antOgre::RenderManager::instance()->getOrCreateSceneManager(sceneName);

	IEventManager* pEventMgr = IEventManager::instance();
	pEventMgr->addListener(MakeDelegate(this, &OgreScene::newRenderComponentDelegate), EvtData_New_RenderComponent::sk_EventType);
	pEventMgr->addListener(MakeDelegate(this, &OgreScene::destroyActorDelegate), EvtData_Destroy_Actor::sk_EventType);
	//pEventMgr->addListener(MakeDelegate(this, &OgreScene::modifiedRenderComponentDelegate), EvtData_Modified_SFMLRender_Component::sk_EventType);
}

OgreScene::~OgreScene()
{
	// We should destroy the scene manager contents here?
	IEventManager* pEventMgr = IEventManager::instance();
	pEventMgr->removeListener(MakeDelegate(this, &OgreScene::newRenderComponentDelegate), EvtData_New_RenderComponent::sk_EventType);
	pEventMgr->removeListener(MakeDelegate(this, &OgreScene::destroyActorDelegate), EvtData_Destroy_Actor::sk_EventType);
	//pEventMgr->removeListener(MakeDelegate(this, &SFMLScene::moveActorDelegate), EvtData_Move_SFMLActor::sk_EventType);
	//pEventMgr->removeListener(MakeDelegate(this, &OgreScene::modifiedRenderComponentDelegate), EvtData_Modified_SFMLRender_Component::sk_EventType);
}

bool OgreScene::onUpdate(ant::DeltaTime const dt)
{
	// Maybe update the position of all nodes?

	return true;
}

void OgreScene::onRender(DeltaTime fTime, DeltaTime fElapsedTime)
{
	// sync all actors transformation components with their scene nodes
	for (auto& i : m_sceneActorMap )
	{
		ActorStrongPtr actor = MakeStrongPtr( ant::IOgreApp::getApp()->getGameLogic()->getActor(i.first));
		if (actor)
		{
			TransformComponentStrongPtr pTrans = MakeStrongPtr(actor->getComponent<TransformComponent>(TransformComponent::g_Name));
			if (pTrans)
			{
				i.second->setPosition(ANT_VEC3_TO_OGRE_VEC3(pTrans->getPosition()));				
			}
		}
	}
}

Ogre::SceneNode* OgreScene::findActor(ant::ActorId actor)
{
	auto it = m_sceneActorMap.find(actor);
	if (it == m_sceneActorMap.end())
	{
		return nullptr;
	}

	return it->second;
}

bool OgreScene::addChild(ant::ActorId id, Ogre::SceneNode* node)
{
	if (id != INVALID_ACTOR_ID)
	{
		m_sceneActorMap[id] = node;
	}

	if (m_sceneMgr->getRootSceneNode() != node->getParent())
	{
		m_sceneMgr->getRootSceneNode()->addChild(node);
	}

	return true;
}

bool OgreScene::removeChild(ant::ActorId id)
{
	if (id == INVALID_ACTOR_ID)
	{
		return false;
	}

	Ogre::SceneNode * node = m_sceneActorMap[id];

	ANT_ASSERT(node);

	m_sceneActorMap.erase(id);

	m_sceneMgr->getRootSceneNode()->removeChild(node);

	return true;
}

void OgreScene::newRenderComponentDelegate(IEventDataStrongPtr eventData)
{
	shared_ptr<EvtData_New_RenderComponent> pEventData = static_pointer_cast<EvtData_New_RenderComponent>(eventData);

	ActorId actorId = pEventData->getActorId();
	IOGRERenderComponent* comp =  pEventData->getRenderComponent();

	ANT_ASSERT(comp);
	ANT_ASSERT(m_sceneMgr);

	Ogre::SceneNode* node = comp->getSceneNode(m_sceneMgr);
	
	// The node is already a child of the scenemanager root node. Maybe we should generate entities instead?
	addChild(actorId, node);
}

void OgreScene::modifiedRenderComponentDelegate(IEventDataStrongPtr eventData)
{

}

void OgreScene::destroyActorDelegate(IEventDataStrongPtr eventData)
{
	shared_ptr<EvtData_Destroy_Actor> pCastEventData = static_pointer_cast<EvtData_Destroy_Actor>(eventData);
	removeChild(pCastEventData->getId());
}

void OgreScene::moveActorDelegate(IEventDataStrongPtr eventData)
{

}