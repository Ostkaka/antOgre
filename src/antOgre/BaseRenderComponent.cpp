#include <antOgre/BaseRenderComponent.hpp>
#include <ant/IEventManager.hpp>
#include <actors/Actor.hpp>
#include <ant/Events.hpp>
#include <actors/TransformComponent.hpp>
#include <ant/templates.hpp>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>

const char* antOgre::BaseEntityRenderComponent::g_Name = "BaseEntityRenderComponent";

int createEntities = 0;

antOgre::BaseOGRERenderComponent::BaseOGRERenderComponent()
{
	m_color     = Ogre::ColourValue(1, 1, 1, 1);
	m_sceneNode = nullptr;
}

bool antOgre::BaseOGRERenderComponent::init(TiXmlElement* pData)
{
	// Color
	TiXmlElement* pColorNode = pData->FirstChildElement("Color");
	if (pColorNode)
		m_color = loadColor(pColorNode);

	return delegateInit(pData);
}

void antOgre::BaseOGRERenderComponent::postInit()
{	
	shared_ptr<ant::EvtData_New_RenderComponent> pEvent(ANT_NEW ant::EvtData_New_RenderComponent(m_pOwner->getId(), this));
	ant::IEventManager::instance()->triggerEvent(pEvent);
}

void antOgre::BaseOGRERenderComponent::onChanged()
{

}

TiXmlElement* antOgre::BaseOGRERenderComponent::generateXml()
{
	// TODO Implement XML export
	return nullptr;
}

Ogre::ColourValue antOgre::BaseOGRERenderComponent::loadColor(TiXmlElement* pColorData)
{
	double MAX_COLOR = 256;

	double r = 1.0;
	double g = 1.0;
	double b = 1.0;
	double a = 1.0;

	pColorData->Attribute("r", &r);
	pColorData->Attribute("g", &g);
	pColorData->Attribute("b", &b);
	pColorData->Attribute("a", &a);

	Ogre::ColourValue color((Ogre::uint8)(r * 255), (Ogre::uint8)(g * 255), (Ogre::uint8)(b * 255), (Ogre::uint8)(a * 255));

	return color;
}

Ogre::SceneNode* antOgre::BaseOGRERenderComponent::getSceneNode(Ogre::SceneManager* mgr)
{
	if (!m_sceneNode)
	{
		m_sceneNode = createSceneNode(mgr);
	}

	return m_sceneNode;
}

//////////////////////////////////////////////////////////////////////////
// BaseEntityRenderComponent
//////////////////////////////////////////////////////////////////////////

antOgre::BaseEntityRenderComponent::BaseEntityRenderComponent()
{
	m_entityName = "";
}

Ogre::SceneNode* antOgre::BaseEntityRenderComponent::createSceneNode(Ogre::SceneManager* mgr)
{
	// Try to get the transform component here. Is this an ugly hack?
	ant::TransformComponentStrongPtr pTransformComponent = MakeStrongPtr(m_pOwner->getComponent<ant::TransformComponent>(ant::TransformComponent::g_Name));
	
	createEntities++;
	
	std::string generatename = m_entityName + ToStr(createEntities);

	Ogre::Entity* entity = mgr->createEntity(generatename, m_entityName);
	Ogre::SceneNode* node = mgr->getRootSceneNode()->createChildSceneNode(generatename + std::string("Node"));
	node->attachObject(entity);
	node->setScale(Ogre::Vector3(0.5));

	if (pTransformComponent)
	{
		ant::Vec3 pos = pTransformComponent->getPosition();

		node->setPosition(pos.x, pos.y, pos.z);
	}
	
	return node;
}

bool antOgre::BaseEntityRenderComponent::delegateInit(TiXmlElement* data)
{
	ANT_ASSERT(data);

	// Get texture resource path	
	TiXmlElement* entityNode = data->FirstChildElement("Entity");
	if (entityNode)
	{
		m_entityName = entityNode->FirstChild()->Value();
	}

	return true;
}