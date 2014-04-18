#ifndef RESOURCE_HPP_
	#define RESOURCE_HPP_

#include <ant/core_types.hpp>

namespace ant 
{
	/**
	 * Class that acts like an identifier for a resource asset in as resource file
	 */
	class Resource
	{
	public:
		
		/// Default constructor that takes the unique identifier for the resource
		Resource(const std::string& name);

		/// Default destructor
		~Resource();

		/// Gets the string identifier for the resource
		std::string getName() const;

		/// Sets the string identifier for the resource
		std::string setName(const std::string& name);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		std::string m_name;
	};

	/**
	 * Class that is used to attach extra data to a resource file.
	 */
	class IResourceExtraData
	{
	public:
		/// Converts the extra data to a string
		virtual std::string toString()=0;
	};

	/* Implementation */
	ANT_INLINE std::string Resource::getName() const{ return m_name;	}

	ANT_INLINE std::string Resource::setName(const std::string& name){ m_name = name; }
}

#endif