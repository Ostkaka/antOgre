#ifndef GENERIC_OBJECT_FACTORY_HPP_
	#define GENERIC_OBJECT_FACTORY_HPP_

#include <ant/ant_std.hpp>
#include <ant/core_types.hpp>

namespace ant
{
	/**
	* This template class is used by the actor factory to help create components 
	* and the creation types of them
	*/
	template <class BaseType, class SubType>
	BaseType* GenericObjectCreationFunction( void ) { return new SubType; }

	template <class BaseClass, class IdType>
	class GenericObjectFactory
	{
		typedef BaseClass* (*ObjectCreationFunction)( void );
		std::map<IdType, ObjectCreationFunction> m_creationFunctions;

	public:
		template <class SubClass>
		bool Register(IdType id)
		{
			auto findIt = m_creationFunctions.find(id);
			if (findIt == m_creationFunctions.end())
			{
				m_creationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;  // insert() is giving me compiler errors
				return true;
			}

			return false;
		}

		BaseClass* Create(IdType id)
		{
			auto findIt = m_creationFunctions.find(id);
			if (findIt != m_creationFunctions.end())
			{
				ObjectCreationFunction pFunc = findIt->second;
				return pFunc();
			}

			return NULL;
		}
	};
}

#endif