#ifndef ACTOR_HPP_
	#define ACTOR_HPP_

#include <string>
#include <ant/core_types.hpp>

// Forward declaration
class TiXmlElement;

namespace ant
{
	class ActorComponent;
	class Actor;
}

namespace ant
{
	// Declare some typedefs
	typedef std::string ActorType;

	class Actor
	{
		friend class ActorFactory;

	public:
		
		typedef std::map<ComponentId, ActorComponentStrongPtr> ActorComponents;

	public:

		explicit Actor(ActorId id);

		// Default Destructor
		~Actor();

		/*
		Init from XML data
		*/
		bool init(TiXmlElement* pData);

		/*
		Init from XML data
		*/
		void postInit();
	
		/*
		Returns the id of the actor
		*/
		ActorId getId(void);

		/*
		Returns the type of the actor
		*/
		ActorType getType();

		/*
		Init from XML data
		*/
		void destroy();
	
		/*
		Init from XML data
		*/
		void update(ant::DeltaTime dt);

		/*
		Converts the data of the actor to XML format
		*/
		std::string Actor::toXML();

		/// TODO - add get Component templates when the class is done

		/*
		Get the components form the actor 
		*/
		const ActorComponents* getComponents();

		/*
		Adds a ActorComponent to the actor
		*/
		void addComponent(ActorComponentStrongPtr pComponent);

		//////////////////////////////////////////////////////////////////////////
		// Template functions
		//////////////////////////////////////////////////////////////////////////

		template <class ComponentType>
		weak_ptr<ComponentType> getComponent(ComponentId id)
		{
			ActorComponents::iterator findIt = m_components.find(id);
			if (findIt != m_components.end())
			{
				ActorComponentStrongPtr pBase(findIt->second);
				shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));  // cast to subclass version of the pointer
				weak_ptr<ComponentType> pWeakSub(pSub);  // convert strong pointer to weak pointer
				return pWeakSub;  // return the weak pointer
			}
			else
			{
				return weak_ptr<ComponentType>();
			}
		}

		template <class ComponentType>
		weak_ptr<ComponentType> getComponent(const char *name)
		{
			ComponentId id = ActorComponent::getIdFromName(name);
			ActorComponents::iterator findIt = m_components.find(id);
			if (findIt != m_components.end())
			{
				ActorComponentStrongPtr pBase(findIt->second);
				shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));  // cast to subclass version of the pointer
				weak_ptr<ComponentType> pWeakSub(pSub);  // convert strong pointer to weak pointer
				return pWeakSub;  // return the weak pointer
			}
			else
			{
				return weak_ptr<ComponentType>();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		/// Id of the actor
		ActorId m_id;

		/// Components of the actor
		ActorComponents m_components;	
	
		/// Type of the actor
		ActorType m_type; 

		/// the XML file from which this actor was initialized (considered the "Archetype" file)
		std::string m_resource;			
	};

	// Implementation 
	ANT_INLINE const Actor::ActorComponents* Actor::getComponents() { return &m_components;}

	ANT_INLINE ant::ActorId ant::Actor::getId( void ) { return m_id; }

	ANT_INLINE ant::ActorType ant::Actor::getType( void )	{ return m_type; }

}
#endif

