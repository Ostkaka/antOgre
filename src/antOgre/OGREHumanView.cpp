#include <antOgre/OGREHumanView.hpp>

using namespace ant;

antOgre::OGREHumanView::OGREHumanView()
{
	m_scene.reset(ANT_NEW antOgre::OgreScene("main"));

	m_viewId = ant_InvalidGameViewId;

	registerAllDelegates();

	m_BaseGameState = GAME_STATE_INIT;

	m_camera = m_scene->getSceneManager()->createCamera("mainCamera"); 

	m_cameraNode = m_scene->getSceneManager()->getRootSceneNode()->createChildSceneNode("CamNode1");

	m_cameraNode->attachObject(m_camera);

	m_cameraNode->setPosition(0, 0, 250);

	m_camera->lookAt(0, 0, 0);

	m_camera->setNearClipDistance(5);

	Ogre::Viewport* vp = antOgre::RenderManager::instance()->getRenderWindow()->addViewport(m_camera);

	// Alter the camera aspect ratio to match the viewport
	m_camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
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