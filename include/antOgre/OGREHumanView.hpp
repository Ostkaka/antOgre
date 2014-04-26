#ifndef OGREHUMANVIEW_HPP_
	#define OGREHUMANVIEW_HPP_

#include <ant/IGameView.hpp>
#include <antLua\ScriptEvent.hpp>
#include <antOgre\OgreScene.hpp>
#include <ant\BaseGameLogic.hpp>

using namespace ant;

namespace antOgre
{
	class OGREHumanView : public IGameView
	{
	public:

		OGREHumanView();

		virtual ~OGREHumanView(void);

		bool loadGame(TiXmlElement *levelData);

		virtual void setControllerActor(ActorId id);

		void gameStateDelegates(IEventDataStrongPtr eventData);

		virtual void onRender(ant::DeltaTime fTime, ant::DeltaTime fElapsedTime) ANT_OVERRIDE;

		virtual GameViewType getType() ANT_OVERRIDE;

		virtual GameViewId getId() const ANT_OVERRIDE;

		virtual void onAttach(GameViewId id, ActorId actorid) ANT_OVERRIDE;

		//virtual bool onMsgProc(sf::Event theEvent) = 0;

		virtual void onUpdate(ant::DeltaTime dt) ANT_OVERRIDE;

	protected:

		virtual bool loadGameDelegate(TiXmlElement* levelData);

	private:

		void registerAllDelegates();

		void removeAllDelegates();

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////

	protected:

		GameViewId            m_viewId;
		ActorId               m_actorId;
		antOgre::OgreSceneStrongPtr   m_scene;
		ant::TimeStamp        m_currentTime;
		ant::TimeStamp        m_lastDrawTime;
		BaseGameState         m_BaseGameState;
		Ogre::Camera*         m_cameraNode;
	};

	ANT_INLINE GameViewId OGREHumanView::getId() const { return m_viewId; }

	ANT_INLINE GameViewType OGREHumanView::getType() { return GameView_Human; }

	ANT_INLINE bool OGREHumanView::loadGameDelegate(TiXmlElement* levelData)
	{
		//pushElement(m_Scene);
		return true;
	}

	ANT_INLINE void OGREHumanView::setControllerActor(ActorId aid) { m_actorId = aid; }

	ANT_INLINE void OGREHumanView::onAttach(GameViewId id, ActorId actorid)
	{
		m_viewId = id;
		m_actorId = actorid;
	}

	//ANT_INLINE void OGREHumanView::pushElement(IScreenElementStrongPtr pElement){ m_ScreenElements.push_front(pElement); }

	//ANT_INLINE void OGREHumanView::removeElement(IScreenElementStrongPtr pElement){ m_ScreenElements.remove(pElement); }

	//ANT_INLINE void OGREHumanView::toggleDebugRender() { m_renderDebug = !m_renderDebug; }
}



#endif