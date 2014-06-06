#ifndef TRANSFORMCOMPONENT_HPP_
	#define TRANSFORMCOMPONENT_HPP_

#include <ant/core_types.hpp>
#include <actors/ActorComponent.hpp>
#include <OGRE\OgreQuaternion.h>

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
		ant::Quat getRotation();

		void setPosition(Vec3 pos);

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

	ANT_INLINE void TransformComponent::setPosition(Vec3 pos)
	{		
		m_transform[3][0] = pos.x;
		m_transform[3][1] = pos.y;
		m_transform[3][2] = pos.z;
	}

	ANT_INLINE ant::Vec3 TransformComponent::getPosition()
	{
		// Try to extract the position from the matrix
		ant::Vec3 vec3 = ant::Vec3(m_transform[3][0], m_transform[3][1], m_transform[3][2]);
		return vec3;
	}

	ANT_INLINE ant::Quat TransformComponent::getRotation()
	{	
		return ant::Quat(m_transform);
	}

#define ANT_VEC3_TO_OGRE_VEC3(vec3) Ogre::Vector3(vec3.x,vec3.y,vec3.z)
#define OGRE_VEC3_TO_ANT_VEC3(vec3) ant::Vec3(vec3.x,vec3.y,vec3.z)
#define OGRE_QUAT_TO_ANT_QUAT(q) ant::Quat(q.w,q.x,q.y,q.z)
#define ANT_QUAT_TO_OGRE_QUAT(q) Ogre::Quaternion(q.w,q.x,q.y,q.z)
}

#endif
