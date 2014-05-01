#include <actors/ActorFactory.hpp>
#include <actors/ActorComponent.hpp>
#include <actors/TransformComponent.hpp>
#include <actors/Actor.hpp>
#include <ant/XmlResource.hpp>
#include <ant/ResourceLoaders.hpp>
#include <antLua/BaseScriptComponent.hpp>
#include <ant/GenericObjectFactory.hpp>
#include <ant/templates.hpp>

#include <tinyxml.h>
#include <actors\TransformComponent.hpp>
#include <antOgre\BaseRenderComponent.hpp>

using namespace ant;

ant::ActorFactory::ActorFactory( void )
{
	m_lastActorId = INVALID_ACTOR_ID;
	
	m_componentFactory.Register<BaseScriptComponent>(ActorComponent::getIdFromName(BaseScriptComponent::g_Name));
	m_componentFactory.Register<TransformComponent>(ActorComponent::getIdFromName(TransformComponent::g_Name));
	m_componentFactory.Register<antOgre::BaseEntityRenderComponent>(ActorComponent::getIdFromName(antOgre::BaseEntityRenderComponent::g_Name));
	m_componentFactory.Register<antOgre::LightRenderComponent>(ActorComponent::getIdFromName(antOgre::LightRenderComponent::g_Name));
	m_componentFactory.Register<antOgre::SkyRenderComponent>(ActorComponent::getIdFromName(antOgre::SkyRenderComponent::g_Name));
	m_componentFactory.Register<antOgre::BoxRenderComponent>(ActorComponent::getIdFromName(antOgre::BoxRenderComponent::g_Name));
	m_componentFactory.Register<antOgre::SphereRenderComponent>(ActorComponent::getIdFromName(antOgre::SphereRenderComponent::g_Name));
}

ant::ActorFactory::~ActorFactory()
{
}

ant::ActorStrongPtr ant::ActorFactory::createActor(const char* actorResource, TiXmlElement* overrides, const Mat4 * initialTransform, const ActorId serversActorId)
{
	// Grab the root XML node
	TiXmlElement* pRoot = XmlResourceLoader::loadAndReturnXmlElement(actorResource);
	if (!pRoot)
	{
		ANT_ERROR("Failed to create actor from resource: " + std::string(actorResource));
		return ActorStrongPtr();
	}

	// Create the actor instance
	ActorId nextActorId = serversActorId;
	if (nextActorId == INVALID_ACTOR_ID)
	{
		nextActorId = getNextActorId();
	}
	ActorStrongPtr pActor(ANT_NEW Actor(nextActorId));

	if (!pActor->init(pRoot))
	{
		ANT_ERROR("Failed to initialize actor: " + std::string(actorResource));
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
		if (initialTransform)
		{
			pTransformComponent->setTransform(*initialTransform);
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
			ANT_ERROR("Component failed to initialize: " + std::string(name));
			return ActorComponentStrongPtr();
		}
	}
	else
	{
		ANT_ERROR("Couldn't find ActorComponent named " + std::string(name));
		return ActorComponentStrongPtr();  // fail
	}

	// pComponent will be NULL if the component wasn't found.  This isn't necessarily an error since you might have a 
	// custom CreateComponent() function in a sub class.
	return pComponent;
}

