#ifndef OGRESCENE_HPP_
	#define OGRESCENE_HPP_

#include <antOgre/RenderManager.hpp>
#include <string>
#include <antLua\ScriptEvent.hpp>

using namespace ant;

namespace antOgre
{
	class OgreScene
	{
		typedef std::map<ActorId, Ogre::SceneNode*> SceneActorMap;

		/// Default constructor
		OgreScene(const std::string& name);

		/// Default destructor
		virtual ~OgreScene();

		bool onUpdate(const ant::DeltaTime dt);

		void onRender(void);

		Ogre::SceneNode* findActor(ant::ActorId actor);

		bool addChild(ant::ActorId id, Ogre::SceneNode* node);

		bool removeChild(ant::ActorId id);

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
}


#endif // OGRESCENE_HPP_