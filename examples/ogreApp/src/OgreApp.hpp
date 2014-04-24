#ifndef OGREAPP_HPP_ 
	#define OGREAPP_HPP_

#include <antOgre/IOgreApp.hpp>

namespace ant
{
	class OgreApp : public IOgreApp
	{
	public:
		/**
		* Load the game
		*/
		virtual void loadGameDelegate(IEventDataStrongPtr eventData);

	protected:

		/**
		* Registers events for the game
		*/
		virtual void registerGameEvents(void) ANT_OVERRIDE;

		/**
		* Registers events for the game
		*/
		virtual void registerGameDelegates(void);

		/**
		* Initializes the game logic and views.
		*/
		virtual BaseGameLogic* initGameLogicAndView(void) ANT_OVERRIDE;
	};
}

#endif // OGREAPP_HPP_