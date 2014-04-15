#include <ant/actors/ActorComponent.hpp>

using namespace ant;

ant::ActorComponent::ActorComponent()
{

}

ant::ActorComponent::~ActorComponent()
{
	this->m_pOwner.reset();
}

void ant::ActorComponent::postInit()
{

}

void ant::ActorComponent::update( ant::DeltaTime dt )
{

}

void ant::ActorComponent::onChanged( void )
{

}

ant::ComponentId ant::ActorComponent::getIdFromName( const std::string& componentStr )
{
	void* rawId = HashedString::hash_name(componentStr.c_str());
	return reinterpret_cast<ComponentId>(rawId);
}