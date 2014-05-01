#include <antOgre/BaseRenderComponent.hpp>
#include <ant/IEventManager.hpp>
#include <actors/Actor.hpp>
#include <ant/Events.hpp>
#include <actors/TransformComponent.hpp>
#include <ant/templates.hpp>
#include <antOgre/Utils.hpp>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>

const char* antOgre::BaseEntityRenderComponent::g_Name = "BaseEntityRenderComponent";
const char* antOgre::SkyRenderComponent::g_Name        = "SkyRenderComponent";
const char* antOgre::LightRenderComponent::g_Name      = "LightRenderComponent";
const char* antOgre::BoxRenderComponent::g_Name        = "BoxRenderComponent";
const char* antOgre::SphereRenderComponent::g_Name     = "SphereRenderComponent";

int createEntities = 0;
int createdLights  = 0;

ant::Real BASE_CUBE_SIZE   = 50.0;
ant::Real BASE_SPHERE_SIZE = 50.0;
ant::Real BASE_PLANE_SIZE  = 200.0;

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
	
	std::string generatename = m_entityName + ToStr(createEntities++);

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

//////////////////////////////////////////////////////////////////////////
// SkyRenderComponent
//////////////////////////////////////////////////////////////////////////
antOgre::SkyRenderComponent::SkyRenderComponent()
{
}

Ogre::SceneNode* antOgre::SkyRenderComponent::createSceneNode(Ogre::SceneManager* mgr)
{
	Ogre::Plane plane;
	plane.d = 1000;
	plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;

	mgr->setSkyDome(true, m_textureName, 5, 8);

	// Just return an empty scene node
	return mgr->getRootSceneNode()->createChildSceneNode();
}

bool antOgre::SkyRenderComponent::delegateInit(TiXmlElement* data)
{
	TiXmlElement * pTexture = data->FirstChildElement("Texture");
	if (pTexture)
	{
		m_textureName = pTexture->FirstChild()->Value();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// LightRenderComponent
//////////////////////////////////////////////////////////////////////////
antOgre::LightRenderComponent::LightRenderComponent(void)
{
}

Ogre::SceneNode* antOgre::LightRenderComponent::createSceneNode(Ogre::SceneManager* mgr)
{
	// Try to get the transform component here. Is this an ugly hack?
	ant::TransformComponentStrongPtr pTransformComponent = MakeStrongPtr(m_pOwner->getComponent<ant::TransformComponent>(ant::TransformComponent::g_Name));

	std::string lightname = "light_" + ToStr(createdLights++);

	// Create a Light and set its position
	Ogre::Light* light     = mgr->createLight(lightname);
	Ogre::SceneNode * node = mgr->getRootSceneNode()->createChildSceneNode();

	node->attachObject(light);	

	if (pTransformComponent)
	{
		light->setPosition(ANT_VEC3_TO_OGRE_VEC3(pTransformComponent->getPosition()));
		std::cout << ANT_VEC3_TO_OGRE_VEC3(pTransformComponent->getPosition()) << std::endl;
	}
	
	// TODO - fix synching of the properties!
	return node;
}

bool antOgre::LightRenderComponent::delegateInit(TiXmlElement* data)
{
	TiXmlElement* pLight = data->FirstChildElement("Light");

	if (pLight)
	{
		double temp;
		TiXmlElement* pAttenuationNode = NULL;
		pAttenuationNode = pLight->FirstChildElement("Attenuation");
		if (pAttenuationNode)
		{
			double temp;
			pAttenuationNode->Attribute("const", &temp);
			m_Props.m_Attenuation[0] = (float)temp;

			pAttenuationNode->Attribute("linear", &temp);
			m_Props.m_Attenuation[1] = (float)temp;

			pAttenuationNode->Attribute("exp", &temp);
			m_Props.m_Attenuation[2] = (float)temp;
		}

		TiXmlElement* pShapeNode = NULL;
		pShapeNode = pLight->FirstChildElement("Shape");
		if (pShapeNode)
		{
			pShapeNode->Attribute("range", &temp);
			m_Props.m_Range = (float)temp;
			pShapeNode->Attribute("falloff", &temp);
			m_Props.m_Falloff = (float)temp;
			pShapeNode->Attribute("theta", &temp);
			m_Props.m_Theta = (float)temp;
			pShapeNode->Attribute("phi", &temp);
			m_Props.m_Phi = (float)temp;
		}
	}
	
	return true;
}


//////////////////////////////////////////////////////////////////////////
// BoxRenderComponent
//////////////////////////////////////////////////////////////////////////

antOgre::BoxRenderComponent::BoxRenderComponent(void) : m_halfVector(0, 0, 0)
{
}

Ogre::SceneNode* antOgre::BoxRenderComponent::createSceneNode(Ogre::SceneManager* mgr)
{
	// How to create a box?
	Ogre::Entity * entity = ogreUtils::createCube(mgr);

	Ogre::SceneNode * node = mgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entity);	
	node->setScale(m_halfVector.x / BASE_CUBE_SIZE, m_halfVector.y / BASE_CUBE_SIZE, m_halfVector.z / BASE_CUBE_SIZE);
	
	// Try to get the transform component here. Is this an ugly hack?
	ant::TransformComponentStrongPtr pTransformComponent = MakeStrongPtr(m_pOwner->getComponent<ant::TransformComponent>(ant::TransformComponent::g_Name));
	if (pTransformComponent)
	{
		node->setPosition(ANT_VEC3_TO_OGRE_VEC3(pTransformComponent->getPosition()));
	}

	return node;
}

bool antOgre::BoxRenderComponent::delegateInit(TiXmlElement* data)
{
	TiXmlElement* box = data->FirstChildElement("Box");

	if (box)
	{
		double x = 0;
		double y = 0;
		double z = 0;

		box->Attribute("x", &x);
		box->Attribute("y", &y);
		box->Attribute("z", &z);

		m_halfVector = ant::Vec3(x, y, z);
	}

	return true;	
}

//////////////////////////////////////////////////////////////////////////
// SphereRenderComponent
//////////////////////////////////////////////////////////////////////////

antOgre::SphereRenderComponent::SphereRenderComponent() : m_radius(0)
{
}

SceneNode* antOgre::SphereRenderComponent::createSceneNode(SceneManager* mgr)
{
	// How to create a box?
	Ogre::Entity * entity = ogreUtils::createSphere(mgr);

	Ogre::SceneNode * node = mgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(entity);
	ant::Real r = m_radius / BASE_SPHERE_SIZE;
	node->scale(r, r, r);

	// Try to get the transform component here. Is this an ugly hack?
	ant::TransformComponentStrongPtr pTransformComponent = MakeStrongPtr(m_pOwner->getComponent<ant::TransformComponent>(ant::TransformComponent::g_Name));
	if (pTransformComponent)
	{
		node->setPosition(ANT_VEC3_TO_OGRE_VEC3(pTransformComponent->getPosition()));
	}

	return node;
}

bool antOgre::SphereRenderComponent::delegateInit(TiXmlElement* data)
{
	TiXmlElement* box = data->FirstChildElement("Sphere");

	if (box)
	{
		double radius = 0;
		box->Attribute("radius", &radius);
		m_radius = radius;
	}

	return true;
}