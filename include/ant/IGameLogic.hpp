/*
* This is the abstract app class in ANT that is used for building applications
*/
#ifndef   IGAMELOGIC_HPP
	#define   IGAMELOGIC_HPP

#include <fstream>
#include <stddef.h>
#include <string.h>
#include <vector>
#include <ant/core_types.hpp>
#include <ant_config.hpp>
#include <actors/Actor.hpp>

namespace ant
{
	typedef std::map<ActorId, ActorStrongPtr> ActorMap;

	class IGameLogic 
	{
		public:
			/// Gets and actor active in the game logic given and id
			virtual ActorWeakPtr getActor(const ActorId id) = 0;
			
			/// Creates an actor form from a actor resource with additional options
			virtual ActorStrongPtr createActor(const std::string &actorResource, 
				TiXmlElement *overrides, 
				const ant::Mat4* initialTransform=NULL, 
				const ActorId serversActorId=INVALID_ACTOR_ID)=0;

			/// Destroys actor in the game logic given an id
			virtual void destroyActor(const ActorId id) = 0;

			/// Loads a game given a path to a resource that describes the level
			virtual bool loadGame(const char* levelResource) = 0;
			
			/// Updates all components in the game logic
			virtual void onUpdate(ant::DeltaTime time, ant::DeltaTime elapsedTime) = 0;

			/// Changes the state in the game logic
			virtual void changeGameState(enum BaseGameState newState) = 0;

			/// Moves an actor given a matrix that describes the movement
			//virtual void moveActor(const ActorId id, Mat4x4 const &mat) = 0;

			/// Gets the active game physics implementation in the game logic

			// Get active actors in the scene
			virtual ActorMap* getActiveActors(void) = 0;

			// Set active level in the game logic
			virtual void setActiveLevel(const std::string& level) = 0;
	};
}

#endif /*CORE_APP_HPP*/