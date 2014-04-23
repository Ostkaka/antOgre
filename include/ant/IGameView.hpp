#ifndef IGAMEVIEW_HPP_
	#define IGAMEVIEW_HPP_

#include <ant/core_types.hpp>

namespace ant
{
	enum GameViewType
	{
		GameView_Human,
		GameView_Remote,
		GameView_AI,
		GameView_Recorder,
		GameView_Other
	};

	ANT_DECLARE_POINTER_TYPES(IGameView)
	typedef std::list<shared_ptr<IGameView> > GameViewList;
	/**
	 * Interfaces for a game view that describes how that game looks for a certain type of participant, from AI to player
	 */
	class IGameView
	{
	public:
		virtual bool onRestore() = 0;

		virtual void onRender(ant::DeltaTime fTime, ant::DeltaTime fElapsedTime) = 0;

		virtual bool onLostDevice() = 0;

		virtual GameViewType getType() = 0;

		virtual GameViewId getId() const = 0;

		virtual void onAttach(GameViewId id, ActorId actorid) = 0;

		//virtual bool onMsgProc(sf::Event theEvent) = 0;

		virtual void onUpdate(ant::DeltaTime dt) = 0;

		virtual ~IGameView(){}
	};
}

#endif