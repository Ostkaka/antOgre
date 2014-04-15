#ifndef ACTORFACTORY_HPP_
	#define ACTORFACTORY_HPP_

#include <SFML/Graphics.hpp>
#include <ant/ant_std.hpp>
#include <ant/core_types.hpp>
#include <ant/interfaces/GenericObjectFactory.hpp>

// Forward declaration
class TiXmlElement;

namespace ant
{
	/*
	This class acts as a factory for actors. 
	*/
	class ActorFactory
	{

	protected:
		GenericObjectFactory<ActorComponent, ComponentId> m_componentFactory;

	public:

		/// Default Constructor
		ActorFactory(void);

		/// Default Destructor
		~ActorFactory();

		/// Create and actor from a resource
		ActorStrongPtr createActor(const char* actorResource, 
			TiXmlElement* overrides, 
			const sf::Vector2f* initPos=NULL, 
			const ant::Real* initRot=NULL, 
			const ActorId serversActorId=INVALID_ACTOR_ID);

		/// Modifies Actors based on an XML file
		void modifyActor(ActorStrongPtr pActor, TiXmlElement* overrides);

		/// Creates a component from an XML description
		virtual ActorComponentStrongPtr createComponent(TiXmlElement *pData);
		
	private:
		ActorId getNextActorId(void);
	
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		/// Id of the last actor that was created
		ActorId m_lastActorId;

	};

	/* Implementation*/

	ANT_INLINE ActorId ActorFactory::getNextActorId(void){ ++m_lastActorId; return m_lastActorId; }
}

#endif