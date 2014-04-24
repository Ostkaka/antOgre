#ifndef IOGRERENDERCOMPONENT_HPP
	#define IOGRERENDERCOMPONENT_HPP

#include <actors/ActorComponent.hpp>
#include <OGRE\OgreSceneNode.h>

namespace ant
{
	class IOGRERenderComponent : public ActorComponent
	{
		// Should this return a render component?	
		virtual Ogre::SceneNode* getSceneNode(void) = 0;
	};
}

#endif