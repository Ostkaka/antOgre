#ifndef IOGRERENDERCOMPONENT_HPP
	#define IOGRERENDERCOMPONENT_HPP

#include <actors/ActorComponent.hpp>

namespace ant
{
	class IOGRERenderComponent : public ActorComponent
	{
		// Should this return a render component?
		virtual SFMLSceneNodeStrongPtr getSceneNode(void) = 0;
	};
}

#endif