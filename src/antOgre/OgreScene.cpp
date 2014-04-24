#include <antOgre/OgreScene.hpp>

using namespace antOgre;

OgreScene::OgreScene(const std::string& sceneName)
{
	// Create a scene manager in the current Render Manager
	m_sceneMgr = antOgre::RenderManager::instance()->getOrCreateSceneManager(sceneName);


}

OgreScene::~OgreScene()
{

}

bool OgreScene::onUpdate(ant::DeltaTime const dt)
{

}

Ogre::SceneNode* OgreScene::findActor(ant::ActorId actor)
{

}

bool OgreScene::addChild(ant::ActorId id, Ogre::SceneNode* scene)
{

}

bool OgreScene::removeChild(ant::ActorId id)
{

}