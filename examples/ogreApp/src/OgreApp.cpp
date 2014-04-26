#include "OgreApp.hpp"
#include "TestGameView.hpp"
#include "TestEvents.hpp"
#include "TestGameLogic.hpp"

using namespace ant;

void OgreApp::loadGameDelegate(IEventDataStrongPtr eventData)
{
	initGameLogicAndView();
}

void OgreApp::registerGameEvents()
{
	registerTestEvents();
}

void OgreApp::registerGameDelegates()
{
}

BaseGameLogic* OgreApp::initGameLogicAndView()
{
	m_gameLogic = ANT_NEW TestGameLogic();
	m_gameLogic->init();
	m_gameLogic->setActiveLevel(m_settings.m_level.c_str());

	shared_ptr<antOgre::TestGameView> playaView(ANT_NEW antOgre::TestGameView());

	m_gameLogic->addGameView(playaView);

	return m_gameLogic;
}

