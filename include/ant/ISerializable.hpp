#ifndef ISERIALIZABLE_HPP_
	#define ISERIALIZABLE_HPP_

#include <strstream>

namespace ant
{
	class ISerializable
	{
	public:
		/// Default virtual constructor
		virtual ~ISerializable() {};

		// TODO - make this a class ISerializable
		virtual void serialize(std::ostrstream& out) const = 0;  

		virtual void deserialize(std::istrstream& in) = 0;
	
	};
}

// Call this in subclasses to define the serialization interface
#define ANT_DECLARE_SERIALIZABLE () \
	virtual void serialize(std::ostrstream& out) const;  \
	virtual void deserialize(std::istrstream& in);

#define ANT_DECLARE_ABSTRACT_SERIALIZABLE () \
	virtual void serialize(std::ostrstream& out) const = 0;  \
	virtual void deserialize(std::istrstream& in) = 0;
#endif