#ifndef IOGRERENDERCOMPONENT_HPP
	#define IOGRERENDERCOMPONENT_HPP

#include <actors/ActorComponent.hpp>
#include <OGRE/OgreSceneNode.h>

namespace antOgre
{
	class IOGRERenderComponent : public ant::ActorComponent
	{
	public:
		// Should this return a render component?	
		virtual Ogre::SceneNode* getSceneNode(Ogre::SceneManager* mgr) = 0;
	};
}

#endif