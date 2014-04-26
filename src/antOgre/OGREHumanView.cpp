#include <antOgre/OGREHumanView.hpp>

using namespace ant;

antOgre::OGREHumanView::OGREHumanView()
{
	m_scene.reset(ANT_NEW antOgre::OgreScene("main"));

	m_viewId = ant_InvalidGameViewId;

	registerAllDelegates();

	m_BaseGameState = GAME_STATE_INIT;

	//m_cameraNode = antOgre::RenderManager::instance()->getOrCreateSceneManager("main")->createCamera(""); 
}

antOgre::OGREHumanView::~OGREHumanView()
{
	m_scene.reset();
}

void antOgre::OGREHumanView::gameStateDelegates(IEventDataStrongPtr eventData)
{

}

void antOgre::OGREHumanView::onRender(DeltaTime fTime, DeltaTime fElapsedTime)
{
	m_currentTime = fTime;
	// Update the scene
	if (m_scene)
	{
		m_scene->onRender(fTime, fElapsedTime);
	}

	m_lastDrawTime = m_currentTime;
}

void antOgre::OGREHumanView::onUpdate(DeltaTime dt)
{
	// Nothing yet
}

bool antOgre::OGREHumanView::loadGame(TiXmlElement* levelData)
{
	// Nothing yet
	return loadGameDelegate(levelData);
}

void antOgre::OGREHumanView::registerAllDelegates()
{
	// Nothing yet
}

void antOgre::OGREHumanView::removeAllDelegates()
{
	// Nothing yet
}