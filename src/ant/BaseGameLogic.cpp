#include <ant/BaseGameLogic.hpp>
#include <actors/ActorFactory.hpp>
#include <ant/Events.hpp>
#include <ant/ResourceCacheManager.hpp>
#include <ant/ResourceCache.hpp>
#include <ant/XmlResource.hpp>
#include <actors/TransformComponent.hpp>
#include <actors/Actor.hpp>
#include <ant/String.hpp>
#include <ant/Events.hpp>
#include <ant/templates.hpp>

#include <iostream>

using namespace ant;

//////////////////////////////////////////////////////////////////////////
//
// BaseGameLogic implementation
//
//////////////////////////////////////////////////////////////////////////

ant::BaseGameLogic::BaseGameLogic()
{
	m_lastActorId = 0;
	m_lifetime = 0;
	m_processManager = ANT_NEW ProcessManager;
	m_gameState = GAME_STATE_INIT;
	m_actorFactory = NULL;
	m_currentLevel = "";

	m_levelManager = ANT_NEW LevelManager;
	ANT_ASSERT(m_processManager && m_levelManager);
	//m_pLevelManager->Initialize(g_pApp->m_ResCache->Match("world\\*.xml")); // This is not good at all. Should not be hardcoded

	registerEngineScriptEvents();
	registerAllDelegates();
}

ant::BaseGameLogic::~BaseGameLogic()
{
	removeAllDelegates();

	// Destroy all game views
	while(!m_gameViews.empty())
		m_gameViews.pop_front();

	// Clear level
	clearLoadedLevel();

	SAFE_DELETE(m_levelManager);
	SAFE_DELETE(m_processManager);
	SAFE_DELETE(m_actorFactory);
}

void ant::BaseGameLogic::clearLoadedLevel()
{
	// Destroy actors	
	for (auto it = m_actors.begin() ; it != m_actors.end() ; ++it)
	{
		it->second->destroy();
	}
	m_actors.clear();
}


bool ant::BaseGameLogic::init( void )
{
	m_actorFactory = createActorFactory();

	return true;
}

bool ant::BaseGameLogic::loadGame( const char* levelResource ) 
{
	// Grab the root node
	TiXmlElement* root = XmlResourceLoader::loadAndReturnXmlElement(levelResource);
	if (!root)
	{
		ANT_ERROR("Failed to find level resource file: " + std::string(levelResource));
		return false;
	}

	// Fix the pre and load scripts
	const char* preLoadScript = NULL;
	const char* postLoadScript = NULL;

	// pre and post load scripts from the level description
	TiXmlElement * scriptElement = root->FirstChildElement("Script");
	if (scriptElement)
	{
		preLoadScript = scriptElement->Attribute("preLoad");
		postLoadScript = scriptElement->Attribute("postLoad");
	}

	// Load the pre-load script if there is one
	if (preLoadScript)
	{
		Resource resource(preLoadScript);
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);
	}

	// init all game views - TODO fix this for OGRE
	/*for (auto it = m_gameViews.begin() ; it != m_gameViews.end() ; ++it)
	{
		shared_ptr<IGameView> pView = *it;
		if (pView->getType() == GameView_Human)
		{
			shared_ptr<SFMLHumanView> pHumanView = static_pointer_cast<SFMLHumanView, IGameView>(pView);
			pHumanView->loadGame(root);
		}
	}*/

	if (!loadGameDelegate(root))
	{
		return false;
	}

	// load all initial actors
	TiXmlElement* pActorsNode = root->FirstChildElement("StaticActors");
	if (pActorsNode)
	{
		for (TiXmlElement* pNode = pActorsNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			const char* actorResource = pNode->Attribute("resource");

			ActorStrongPtr pActor = createActor(actorResource, pNode);
			if (pActor)
			{
				shared_ptr<EvtData_New_Actor> pNewActorEvent(ANT_NEW EvtData_New_Actor(pActor->getId()));
				IEventManager::instance()->queueEvent(pNewActorEvent);
			}
		}
	}

	if (postLoadScript)
	{
		Resource resource(postLoadScript);
		// THis will automatically load the post script to the lua manager
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);
	}

	// trigger the Environment Loaded Game event - only then can player actors and AI be spawned!
	shared_ptr<EvtData_Environment_Loaded> pEvent(ANT_NEW EvtData_Environment_Loaded);
	IEventManager::instance()->triggerEvent(pEvent);

	return true;
}

void ant::BaseGameLogic::addGameView( IGameViewStrongPtr pView, ActorId actorid )
{
	int viewId = static_cast<int>(m_gameViews.size());
	m_gameViews.push_back(pView);
	pView->onAttach(viewId,actorid);
	pView->onRestore();
}

void ant::BaseGameLogic::removeGameView( IGameViewStrongPtr pView )
{
	m_gameViews.remove(pView);
}

bool ant::BaseGameLogic::loadGameDelegate( TiXmlElement* levelData )
{
	return true;
}

void ant::BaseGameLogic::onUpdate( ant::DeltaTime time, ant::DeltaTime elapsedTime )
{
	ant::DeltaTime dt = elapsedTime;
	m_lifetime += dt;

	switch(m_gameState)
	{
		case GAME_STATE_INIT:
			// Now, let's load the environment
			changeGameState(GAME_STATE_LOADING_ENVIRONMENT);
			break;

		case GAME_STATE_SPAWNING_ACTORS:
			changeGameState(GAME_STATE_RUNNING);
			break;

		case GAME_STATE_RUNNING:
			m_processManager->updateProcesses(dt);
			
			break;

		default:
			ANT_ERROR("Unrecognized state.");
	}

	// Update all game views
	for (GameViewList::iterator it = m_gameViews.begin() ; it != m_gameViews.end() ; ++it)
	{
		(*it)->onUpdate(dt);
	}

	// Update game actors
	for (ActorMap::const_iterator it = m_actors.begin() ; it != m_actors.end() ; ++it)
	{
		it->second->update(dt);
	}
}

void ant::BaseGameLogic::changeGameState( BaseGameState newState )
{
	if (newState == GAME_STATE_LOADING_ENVIRONMENT)
	{
		m_gameState = newState;
		if (!this->loadGame(m_currentLevel.c_str()))
		{
			ANT_ERROR("The game failed to load.");			
		}
		else
		{
			changeGameState(GAME_STATE_SPAWNING_ACTORS);
			return;
		}		
	}

	m_gameState = newState;
}

void ant::BaseGameLogic::toggleRenderDiagnostics()
{

}

void ant::BaseGameLogic::renderDiagnostics()
{
	if (m_renderDiagnostics)
	{
		//m_gamePhysics->renderDiagnostics(); Not implemented yet
	}
}

ant::ActorStrongPtr ant::BaseGameLogic::createActor( const std::string &actorResource, TiXmlElement *overrides, const ant::Mat4* initialtransform, const ActorId serversActorId/*=INVALID_ACTOR_ID*/ )
{
	ANT_ASSERT(m_actorFactory);
	
	ActorStrongPtr pActor = m_actorFactory->createActor(actorResource.c_str(), overrides, initialtransform, serversActorId);

	if (pActor)
	{
		m_actors.insert(std::make_pair(pActor->getId(), pActor));
		return pActor;
	}
	else
	{
		ANT_WARNING("Could not create actor");
		return ActorStrongPtr();
	}
}

ant::ActorWeakPtr ant::BaseGameLogic::getActor( const ActorId id )
{
	ActorMap::iterator it = m_actors.find(id);
	if (it != m_actors.end())
	{
		return it->second;
	}
	return ActorWeakPtr();
}

void ant::BaseGameLogic::modifyActor( const ActorId id, TiXmlElement* overrides )
{
	ANT_ASSERT(m_actorFactory);
	if (!m_actorFactory)
	{
		return;
	}
	ActorMap::iterator it = m_actors.find(id);
	if (it != m_actors.end())
	{
		m_actorFactory->modifyActor(it->second, overrides);		
	}
}

void ant::BaseGameLogic::destroyActor( const ActorId id )
{
	// We need to trigger a synchronous event to ensure that any systems responding to this event can still access a 
	// valid actor if need be.  The actor will be destroyed after this.
	shared_ptr<EvtData_Destroy_Actor> pEvent(ANT_NEW EvtData_Destroy_Actor(id));
	IEventManager::instance()->triggerEvent(pEvent);

	ActorMap::iterator it = m_actors.find(id);
	if (it != m_actors.end())
	{
		it->second->destroy();
		m_actors.erase(it);
	}
}

std::string ant::BaseGameLogic::getActorXml( const ActorId id )
{
	ActorStrongPtr pActor = MakeStrongPtr(getActor(id));
	if (pActor)
	{
		return pActor->toXML();
	}
	else
	{
		ANT_ERROR("Couldn't find actor: " + ToStr(id));
	}

	return std::string();
}

void ant::BaseGameLogic::moveActorDelegate( IEventDataStrongPtr pEventData )
{
	//shared_ptr<EvtData_Move_SFMLActor> pCastEventData = static_pointer_cast<EvtData_Move_SFMLActor>(pEventData);
	//moveActor(pCastEventData->getId(), pCastEventData->getPosition(), pCastEventData->getRotation());
}

void ant::BaseGameLogic::registerAllDelegates()
{
	//IEventManager* pGlobalEventManager = IEventManager::instance();	
	//pGlobalEventManager->addListener(MakeDelegate(this, &BaseGameLogic::moveActorDelegate), EvtData_Move_SFMLActor::sk_EventType);
}

void ant::BaseGameLogic::removeAllDelegates()
{
	//IEventManager* pGlobalEventManager = IEventManager::instance();		
	//pGlobalEventManager->removeListener(MakeDelegate(this, &BaseGameLogic::moveActorDelegate), EvtData_Move_SFMLActor::sk_EventType);
}
/*
void ant::BaseGameLogic::moveActor( const ActorId id, const sf::Vector2f& pos, const ant::Real& rotation )
{
	// Try to get the transform component for the actor and move
	ActorStrongPtr pActor = MakeStrongPtr(getActor(id));
	if (pActor)
	{
		TransformComponentStrongPtr pTransform = MakeStrongPtr(pActor->getComponent<TransformComponent>(TransformComponent::g_Name));
		if (pTransform)
		{			
			pTransform->setPosition(pos);
			pTransform->setRotation(rotation);
		}
	}
	else
	{
		GCC_WARNING("Trying to move actor with id: " + ToStr(id) + " but could not find it in gamne logic!" );
	}
}*/

ActorFactory* BaseGameLogic::createActorFactory(void)
{
	return ANT_NEW ActorFactory;
}