#include <ant/actors/Actor.hpp>
#include <ant/actors/ActorComponent.hpp>
#include <ant/core_types.hpp>
#include <tinyxml.h>

using namespace ant;

ant::Actor::Actor( ActorId id )
	:m_id(id),m_type("Unknown"),m_resource("Unknown")
{
}

ant::Actor::~Actor()
{
	GCC_LOG("Actor", std::string("Destroying Actor ") + ToStr(m_id));
	GCC_ASSERT(m_components.empty());  
}

bool ant::Actor::init( TiXmlElement* pData )
{
	GCC_LOG("Actor", std::string("Initializing Actor ") + ToStr(m_id));

	m_type = pData->Attribute("type");
	m_resource = pData->Attribute("resource");
	return true;
}

void ant::Actor::postInit()
{
	for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->postInit();
	}
}

void ant::Actor::destroy()
{
	m_components.clear();
}

void ant::Actor::update(ant::DeltaTime dt)
{
	for (ActorComponents::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->update(dt);
	}
}

std::string ant::Actor::toXML()
{
	TiXmlDocument outDoc;

	// Actor element
	TiXmlElement* pActorElement = GCC_NEW TiXmlElement("Actor");
	pActorElement->SetAttribute("type", m_type.c_str());
	pActorElement->SetAttribute("resource", m_resource.c_str());

	// components
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		ActorComponentStrongPtr pComponent = it->second;
		TiXmlElement* pComponentElement = pComponent->generateXml();
		pActorElement->LinkEndChild(pComponentElement);
	}

	outDoc.LinkEndChild(pActorElement);
	TiXmlPrinter printer;
	outDoc.Accept(&printer);

	return printer.CStr();
}

void ant::Actor::addComponent( ActorComponentStrongPtr pComponent )
{
	std::pair<ActorComponents::iterator, bool> success = m_components.insert(std::make_pair(pComponent->getId(), pComponent));
	GCC_ASSERT(success.second);
}

