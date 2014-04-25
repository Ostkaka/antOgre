#include <actors/TransformComponent.hpp>
#include <iostream>

using namespace ant;

const char* TransformComponent::g_Name = "TransformComponent";

bool TransformComponent::init(TiXmlElement* pData)
{
	Vec3 position;
	Vec3 yawPitchRoll;

	TiXmlElement* pPositionElement = pData->FirstChildElement("Position");
	if (pPositionElement)
	{
		double x = 0;
		double y = 0;
		double z = 0;
		pPositionElement->Attribute("x", &x);
		pPositionElement->Attribute("y", &y);
		pPositionElement->Attribute("z", &z);
		position = Vec3(x, y, z);
	}
	
	TiXmlElement* pOrientationElement = pData->FirstChildElement("YawPitchRoll");
	if (pOrientationElement)
	{
		double yaw = 0;
		double pitch = 0;
		double roll = 0;
		pOrientationElement->Attribute("x", &yaw);
		pOrientationElement->Attribute("y", &pitch);
		pOrientationElement->Attribute("z", &roll);
		yawPitchRoll = Vec3(yaw, pitch, roll);
	}

	Mat4 translation = glm::translate( Mat4(1.0f), position);

	Mat4 rotation = glm::rotate(Mat4(1.0f), yawPitchRoll.x, Vec3(1.0, 0, 0)) * glm::rotate(Mat4(1.0f), yawPitchRoll.y, Vec3(0, 1.0, 0)) * glm::rotate(Mat4(1.0f), yawPitchRoll.z, Vec3(0, 0, 1.0));

	m_transform = translation * rotation;

	return true;
}

TiXmlElement* TransformComponent::generateXml()
{
	return NULL;
}

ant::Vec3 TransformComponent::getPosition()
{
	// Try to extract the position from the matrix
	ant::Vec3 vec3 = ant::Vec3(m_transform[0][4], m_transform[1][4], m_transform[2][4]);

	std::cout << "Position: " << vec3.x << " " << vec3.y << " " << vec3.z << std::endl;

	return vec3;
}