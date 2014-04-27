#ifndef OGRESCENE_HPP_
	#define OGRESCENE_HPP_

#include <antOgre/RenderManager.hpp>
#include <string>
#include <antLua\ScriptEvent.hpp>

using namespace ant;

namespace antOgre
{
	ANT_DECLARE_POINTER_TYPES(OgreScene)

	class OgreScene
	{
		typedef std::map<ActorId, Ogre::SceneNode*> SceneActorMap;
	public:
		/// Default constructor
		OgreScene(const std::string& name);

		/// Default destructor
		virtual ~OgreScene();

		bool onUpdate(const ant::DeltaTime dt);

		void onRender(DeltaTime fTime, DeltaTime fElapsedTime);

		Ogre::SceneNode* findActor(ant::ActorId actor);

		bool addChild(ant::ActorId id, Ogre::SceneNode* node);

		bool removeChild(ant::ActorId id);

		Ogre::SceneManager* getSceneManager();

		/// delegates
		void newRenderComponentDelegate(IEventDataStrongPtr eventData);
		void modifiedRenderComponentDelegate(IEventDataStrongPtr eventData);
		void destroyActorDelegate(IEventDataStrongPtr eventData);
		void moveActorDelegate(IEventDataStrongPtr eventData);
		
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		Ogre::SceneManager* m_sceneMgr;
		SceneActorMap       m_sceneActorMap;
	};

	ANT_INLINE Ogre::SceneManager* OgreScene::getSceneManager()
	{
		ANT_ASSERT(m_sceneMgr);
		return m_sceneMgr; 
	}
}


#endif // OGRESCENE_HPP_