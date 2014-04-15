#ifndef ACTORCOMPONENT_HPP_
	#define ACTORCOMPONENT_HPP_

#include <string>
#include <ant/ant_std.hpp>
#include <ant/core_types.hpp>
#include <ant/actors/Actor.hpp>
#include <ant/gccUtils/String.hpp>

// Forward declarations
class TiXmlElement;

namespace ant 
{

	/**
	 * This class describes an actor component that can be coupled to an actor, such as physics, sound, AI, etc....
	 */
	class ActorComponent
	{
		friend class ActorFactory;

	public:
		
		/// Default Constructor
		ActorComponent();

		/// Default Destructor
		virtual ~ActorComponent();
	
		virtual bool init(TiXmlElement* pData) = 0;

		virtual TiXmlElement* generateXml(void) = 0;

		virtual const char* getName() const = 0;

		virtual ComponentId getId(void) const;

		virtual void postInit();

		virtual void update(ant::DeltaTime dt);

		virtual void onChanged(void);		

		static ComponentId getIdFromName(const std::string& componentStr);

		ActorWeakPtr getOwner() const;

	private:

		void setOwner(ActorStrongPtr actor);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		// Actor that owns this component
		ActorStrongPtr m_pOwner;
	};

	/// Implementation
	ANT_INLINE void ant::ActorComponent::setOwner(ActorStrongPtr actor){m_pOwner = actor;}

	ANT_INLINE ant::ComponentId ant::ActorComponent::getId( void ) const { return getIdFromName(getName());}

	ANT_INLINE ant::ActorWeakPtr ant::ActorComponent::getOwner( void ) const { return m_pOwner;}

}

#endif