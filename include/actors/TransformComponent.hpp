#ifndef TRANSFORMCOMPONENT_HPP_
	#define TRANSFORMCOMPONENT_HPP_

#include <ant/core_types.hpp>
#include <actors/ActorComponent.hpp>

namespace ant
{
	ANT_DECLARE_POINTER_TYPES(TransformComponent)

	class TransformComponent : public ActorComponent
	{
	public:
		TransformComponent(void) : m_transform(Mat4()) { }

		virtual bool init(TiXmlElement* pData) ANT_OVERRIDE;
		
		virtual TiXmlElement* generateXml(void) ANT_OVERRIDE;

		Mat4 getTransform(void) const;

		void setTransform(const Mat4& newTransform);

		virtual const char* getName() const ANT_OVERRIDE { return g_Name; }
		
		// TODO - fix the get/set position stuff
		ant::Vec3 getPosition();

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	public:
		static const char* g_Name;
	protected:	
		Mat4               m_transform;
	};

	/* Implementation */

	ANT_INLINE Mat4 TransformComponent::getTransform(void) const { return m_transform; }

	ANT_INLINE void TransformComponent::setTransform(const Mat4& newTransform) { m_transform = newTransform; }
}

#endif
