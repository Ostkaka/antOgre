#ifndef OGRESCENE_HPP_
	#define OGRESCENE_HPP_

#include <antOgre/RenderManager.hpp>
#include <string>

namespace antOgre
{
	class OgreScene
	{
		/// Default constructor
		OgreScene(const std::string& name);

		/// Default destructor
		virtual ~OgreScene();

		bool onUpdate(const ant::DeltaTime dt);

		Ogre::SceneNode* findActor(ant::ActorId actor);

		bool addChild(ant::ActorId id, Ogre::SceneNode* scene);

		bool removeChild(ant::ActorId id);

		/// Fix delegates

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		Ogre::SceneManager* m_sceneMgr;

	};
}


#endif // OGRESCENE_HPP_