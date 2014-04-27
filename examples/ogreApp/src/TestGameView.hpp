#ifndef TESTGAMEVIEW_HPP_
	#define TESTGAMEVIEW_HPP_

#include <antOgre\OGREHumanView.hpp>
#include "TestController.hpp"

namespace antOgre
{
	/**
	* Test Game view TODO - should send in arguments that specify what scenegraph it should be coupled to
	*/
	class TestGameView : public OGREHumanView
	{
	public:
		TestGameView();

		virtual ~TestGameView();

		// Controller
		virtual void setControllerActor(ActorId actorId);

		virtual void onAttach(GameViewId id, ActorId actorid) ANT_OVERRIDE;

		virtual void onUpdate(ant::DeltaTime dt) ANT_OVERRIDE;

		virtual void setControllerActorDelegate(IEventDataStrongPtr eventPtr);

	protected:

		virtual bool loadGameDelegate(TiXmlElement* levelData) ANT_OVERRIDE;

	private:

		/// Delegates
		void registerAllDelegates(void);

		void removeAllDelegates(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		ant::Real                   m_zoomFactor;
		TestControllerStrongPtr     m_controller;

		// Main camera
		Ogre::Camera *              m_camera;
		Ogre::SceneNode*	        m_cameraNode;
	};
}



#endif