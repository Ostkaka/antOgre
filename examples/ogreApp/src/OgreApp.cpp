#include "OgreApp.hpp"

using namespace ant;

void OgreApp::loadGameDelegate(IEventDataStrongPtr eventData)
{
	initGameLogicAndView();
}

void OgreApp::registerGameEvents()
{

}

void OgreApp::registerGameDelegates()
{
}

BaseGameLogic* OgreApp::initGameLogicAndView()
{
	m_gameLogic = ANT_NEW BaseGameLogic();
	m_gameLogic->init();
	m_gameLogic->setActiveLevel(m_settings.m_level.c_str());
		
	return m_gameLogic;
}