#ifndef UTILS_HPP_
	#define UTILS_HPP_

#include <OGRE\OgreEntity.h>
#include <OGRE\OgreMesh.h>
#include <OGRE\OgreSubMesh.h>
#include <OGRE\OgreMeshManager.h>
#include <OGRE\OgreSceneManager.h>
#include <ant\GenericObjectFactory.hpp>
#include <ant\String.hpp>

using namespace Ogre;

namespace ogreUtils
{
	/**
	* Create a box to render
	*/
	Ogre::Entity* createBox(const ant::Vec3& halfvector, Ogre::SceneManager * mgr);

	/**
	* Create cube
	*/
	Ogre::Entity* createCube(Ogre::SceneManager * mgr);

	/**
	* Create sphere
	*/
	Ogre::Entity* createSphere(Ogre::SceneManager * mgr);

	/**
	* Create sphere
	*/
	Ogre::Entity* createPlane(Ogre::SceneManager * mgr);
}

#endif // UTILS_HPP_