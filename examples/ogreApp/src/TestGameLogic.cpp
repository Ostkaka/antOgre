#include "TestGameLogic.hpp"
#include <ant/Events.hpp>
#include <ant/BaseGameLogic.hpp>
#include <ant/String.hpp>
#include <ant/ant_std.hpp>
#include <ant/templates.hpp>
#include "TestEvents.hpp"

using namespace ant;

ant::TestGameLogic::TestGameLogic()
{
	registerAllDelegates();
	m_HumanPlayersAttached = 0;
	m_AIPlayersAttached = 0;
}

ant::TestGameLogic::~TestGameLogic()
{
	removeAllDelegates();
}

void ant::TestGameLogic::changeGameState(BaseGameState newState)
{
	BaseGameLogic::changeGameState(newState);
}

void ant::TestGameLogic::addGameView(IGameViewStrongPtr pView, ActorId actorId/*=INVALID_ACTOR_ID*/)
{
	// Do basic initialization in base class
	BaseGameLogic::addGameView(pView, actorId);
}

void ant::TestGameLogic::TestScriptDelegate(IEventDataStrongPtr pEventData)
{
	shared_ptr<EvtData_TestFromLua> pCastEventData = static_pointer_cast<EvtData_TestFromLua>(pEventData);
	ANT_LOG("Lua", "Event received in C++ from Lua: " + ToStr(pCastEventData->getNumber()));
}

bool ant::TestGameLogic::loadGameDelegate(TiXmlElement* levelData)
{
	registerTestEvents();
	return true;
}

void ant::TestGameLogic::registerAllDelegates(void)
{
	IEventManager* pGlobalEventManager = IEventManager::instance();
	pGlobalEventManager->addListener(MakeDelegate(this, &TestGameLogic::TestScriptDelegate), EvtData_TestFromLua::sk_EventType);
}

void ant::TestGameLogic::removeAllDelegates(void)
{
	IEventManager* pGlobalEventManager = IEventManager::instance();
	pGlobalEventManager->addListener(MakeDelegate(this, &TestGameLogic::TestScriptDelegate), EvtData_TestFromLua::sk_EventType);
}

