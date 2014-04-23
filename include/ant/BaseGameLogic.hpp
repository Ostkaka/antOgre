#ifndef BASEGAMELOGIC_HPP_
	#define BASEGAMELOGIC_HPP_

#include <ant/core_types.hpp>
#include <ant/ProcessManager.hpp>
#include <ant/IGameLogic.hpp>
#include <ant/IGameView.hpp>
#include <antLua\ScriptEvent.hpp>
#include <actors\Actor.hpp>

class GCCRandom;

namespace ant
{
	/// Just some temporary enums till a better solution comes around
	enum BaseGameState
	{
		GAME_STATE_INVALID,
		GAME_STATE_INIT,
		GAME_STATE_LOADING_ENVIRONMENT,
		GAME_STATE_SPAWNING_ACTORS,
		GAME_STATE_RUNNING
	};

	class LevelManager;
	class ActorFactory;

	typedef std::string Level;

	/// How to use this?
	class LevelManager
	{
	public:
		const std::vector<Level> &getLevels() const { return m_Levels; }
		const int getCurrentLevel() const { return m_CurrentLevel; }
		bool initialize(std::vector<std::string> &levels);

	protected:
		std::vector<Level>	m_Levels;
		int									m_CurrentLevel;
	};

	/**
	* Basic implementation of the basic game logic implementation
	*/
	class BaseGameLogic : public IGameLogic
	{

		friend class ISFMLApp; // Opps this should be abstracted to base App class for SFML, DirectX, etc...

	public:

		BaseGameLogic(void);

		virtual ~BaseGameLogic();

		bool init(void);

		/// TODO - make Proxy functions

		/// Generate a new unique actor id
		ActorId getNewActorId(void);

		/// Returns the random number generator inside the game logic
		GCCRandom& getRNG(void);

		/// Adds a game view to the game logic
		virtual void addGameView(IGameViewStrongPtr pView, ActorId actorid=INVALID_ACTOR_ID);

		/// Removes a game view to the game logic
		virtual void removeGameView(IGameViewStrongPtr pView);

		/// Create an actor
		virtual ActorStrongPtr createActor( const std::string &actorResource, 
			TiXmlElement *overrides=NULL, 
			const ant::Mat4* initialTransform = NULL,
			const ActorId serversActorId=INVALID_ACTOR_ID);

		/// Destroy an actor given an id
		virtual void destroyActor(const ActorId id);

		/// Get an actor given an id
		virtual ActorWeakPtr getActor(const ActorId id);

		/// Modify an actor given an id and override it with given XML data
		virtual void modifyActor(const ActorId id, TiXmlElement* overrides);

		/// Loads a game
		virtual bool loadGame(const char* levelResource) ANT_OVERRIDE;

		/// Logic update function
		virtual void onUpdate(ant::DeltaTime time, ant::DeltaTime elapsedTime);
				
		/// Function used to move actor in the game logic
		//virtual void moveActor(const ActorId id, const sf::Vector2f& pos, const ant::Real& rotation);

		/// Gets the level manager of the game logic
		const LevelManager* getLevelManager();

		/// Get game views from the logic
		GameViewList* getGameViews();

		/// Change Game logic state 
		virtual void changeGameState(BaseGameState newState);
		const BaseGameState getState() const;

		/// Game physics
		void toggleRenderDiagnostics();  /// Should this even be here?
		virtual void renderDiagnostics();

		/// Attach a process into the process manager inside the game logic
		void attachProcesses(IProcessStrongPtr process);

		// editor functions
		std::string getActorXml(const ActorId id);

		// Get active actors in the scene
		ActorMap* getActiveActors(void);

		// Set active level in the game logic
		virtual void setActiveLevel(const std::string& level);

	protected:
		/// Create an actor factory
		virtual ActorFactory* createActorFactory(void);

		/// Delegate function to load a game given XML data
		virtual bool loadGameDelegate(TiXmlElement* levelData);

		/// Move actor delegate
		void moveActorDelegate(IEventDataStrongPtr pEventData);

		/// Clears actors in level data
		virtual void clearLoadedLevel();

	private:
		void registerAllDelegates();
		void removeAllDelegates();

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		/// The process manager of the game logic
		ProcessManager* m_processManager;
		/// Total lifetime of the game logic
		ant::DeltaTime  m_lifetime;
		/// Map that holds all the actors in the game logic
		ActorMap        m_actors;
		/// Last id used for a new actor in the logic
		ActorId         m_lastActorId;
		/// Factory responsible for creating actors
		ActorFactory*   m_actorFactory;
		/// Lists of different game views connected to the logic
		GameViewList    m_gameViews;
		/// Game state
		BaseGameState   m_gameState;
		/// Game physics
		bool            m_renderDiagnostics;
		/// The level manager of the game
		LevelManager*   m_levelManager;
		// Current string to resource level
		std::string     m_currentLevel;
	};

	ANT_INLINE const BaseGameState BaseGameLogic::getState(void) const { return m_gameState; }

	ANT_INLINE ActorId BaseGameLogic::getNewActorId(void) { return m_lastActorId++; }

	ANT_INLINE const LevelManager* BaseGameLogic::getLevelManager(void) { return m_levelManager; }

	ANT_INLINE void BaseGameLogic::attachProcesses(IProcessStrongPtr process) 
	{
		if (m_processManager)
		{
			m_processManager->attachProcess(process);
		}
	}

	ANT_INLINE GameViewList* BaseGameLogic::getGameViews(){ return &m_gameViews; }

	ANT_INLINE ActorMap* BaseGameLogic::getActiveActors(){ return &m_actors; }

	ANT_INLINE void BaseGameLogic::setActiveLevel(const std::string& level){ m_currentLevel = level;}
}

#endif