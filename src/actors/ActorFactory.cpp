#include <ant/actors/ActorFactory.hpp>
#include <ant/actors/ActorComponent.hpp>
#include <ant/actors/Actor.hpp>
#include <ant/resources/XmlResource.hpp>
#include <ant/resources/ResourceLoaders.hpp>
#include <ant/actors/BaseScriptComponent.hpp>
#include <ant/actors/TransformComponent.hpp>
#include <ant/graphicsSFML/SFMLRenderComponent.hpp>
#include <ant/2DPhysics/PhysicsComponent.hpp>
#include <ant/actors/AnimationComponent.hpp>
#include <ant/gccUtils/templates.hpp>

#include <tinyxml.h>

using namespace ant;

ant::ActorFactory::ActorFactory( void )
{
	m_lastActorId = INVALID_ACTOR_ID;
	
	m_componentFactory.Register<BaseScriptComponent>(ActorComponent::getIdFromName(BaseScriptComponent::g_Name));
	m_componentFactory.Register<TransformComponent>(ActorComponent::getIdFromName(TransformComponent::g_Name));	
	m_componentFactory.Register<PhysicsComponent>(ActorComponent::getIdFromName(PhysicsComponent::g_Name));

	m_componentFactory.Register<SFMLSpriteComponent>(ActorComponent::getIdFromName(SFMLSpriteComponent::g_Name));
	m_componentFactory.Register<SFMLAnimatedSpriteComponent>(ActorComponent::getIdFromName(SFMLAnimatedSpriteComponent::g_Name));
	m_componentFactory.Register<SFMLBackgroundSpriteComponent>(ActorComponent::getIdFromName(SFMLBackgroundSpriteComponent::g_Name));	
	m_componentFactory.Register<SFMLRectanglePrimitiveComponent>(ActorComponent::getIdFromName(SFMLRectanglePrimitiveComponent::g_Name));	
	m_componentFactory.Register<SFMLCirclePrimitiveComponent>(ActorComponent::getIdFromName(SFMLCirclePrimitiveComponent::g_Name));
	m_componentFactory.Register<AnimationComponent>(ActorComponent::getIdFromName(AnimationComponent::g_Name));
}

ant::ActorFactory::~ActorFactory()
{

}

ant::ActorStrongPtr ant::ActorFactory::createActor(const char* actorResource, TiXmlElement* overrides, const sf::Vector2f* initPos, const ant::Real* initRot, const ActorId serversActorId)
{
	// Grab the root XML node
	TiXmlElement* pRoot = XmlResourceLoader::loadAndReturnXmlElement(actorResource);
	if (!pRoot)
	{
		GCC_ERROR("Failed to create actor from resource: " + std::string(actorResource));
		return ActorStrongPtr();
	}

	// Create the actor instance
	ActorId nextActorId = serversActorId;
	if (nextActorId == INVALID_ACTOR_ID)
	{
		nextActorId = getNextActorId();
	}
	ActorStrongPtr pActor(GCC_NEW Actor(nextActorId));

	if (!pActor->init(pRoot))
	{
		GCC_ERROR("Failed to initialize actor: " + std::string(actorResource));
		return ActorStrongPtr();
	}

	bool initialTransformSet = false;

	// Loop through each child element and load the component
	for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		ActorComponentStrongPtr pComponent(createComponent(pNode));
		if (pComponent)
		{
			pActor->addComponent(pComponent);
			pComponent->setOwner(pActor);
		}
		else
		{
			// If an error occurs, we kill the actor and bail.  We could keep going, but the actor is will only be 
			// partially complete so it's not worth it.  Note that the pActor instance will be destroyed because it
			// will fall out of scope with nothing else pointing to it.
			return ActorStrongPtr();
		}
	}

	if (overrides)
	{
		modifyActor(pActor, overrides);
	}

	// This is a bit of a hack to get the initial transform of the transform component set before the 
	// other components (like PhysicsComponent) read it.
	shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->getComponent<TransformComponent>(TransformComponent::g_Name));
	if (pTransformComponent)
	{
		if (initPos)
		{
			pTransformComponent->setPosition(*initPos);
		}

		if (initRot)
		{
			pTransformComponent->setRotation(*initRot);
		}
	}

	// Now that the actor has been fully created, run the post init phase
	pActor->postInit();

	return pActor;

}

void ant::ActorFactory::modifyActor( ActorStrongPtr pActor, TiXmlElement* overrides )
{
	// Loop through each child element and load the component
	for (TiXmlElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		ComponentId componentId = ActorComponent::getIdFromName(pNode->Value());
		ActorComponentStrongPtr pComponent = MakeStrongPtr(pActor->getComponent<ActorComponent>(componentId));
		if (pComponent)
		{
			pComponent->init(pNode);
			pComponent->onChanged();		
		}
		else
		{
			pComponent = createComponent(pNode);
			if (pComponent)
			{
				pActor->addComponent(pComponent);
				pComponent->setOwner(pActor);
			}
		}
	}		
}

ant::ActorComponentStrongPtr ant::ActorFactory::createComponent( TiXmlElement *pData )
{
	const char* name = pData->Value();
	ActorComponentStrongPtr pComponent(m_componentFactory.Create(ActorComponent::getIdFromName(name)));

	// initialize the component if we found one
	if (pComponent)
	{
		if (!pComponent->init(pData))
		{
			GCC_ERROR("Component failed to initialize: " + std::string(name));
			return ActorComponentStrongPtr();
		}
	}
	else
	{
		GCC_ERROR("Couldn't find ActorComponent named " + std::string(name));
		return ActorComponentStrongPtr();  // fail
	}

	// pComponent will be NULL if the component wasn't found.  This isn't necessarily an error since you might have a 
	// custom CreateComponent() function in a sub class.
	return pComponent;
}

