/**
 * Provides the ANT namespace includes for the ANT Core includes and
 * variables.
 */

#ifndef   ANT_CORE_TYPES_HPP
    #define   ANT_CORE_TYPES_HPP

/////////////////////////////////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////////////////////////////////
//#include <MGE/ANT_config.hpp>
#include <map>
#include <string>
#include <vector>
#include <ant_config.hpp>
#include <ant/ant_std.hpp>

//define the resource dir
//#define _RELEASE_GAME
#ifndef _RELEASE_GAME
    #define RESOURCE_DIR "D:/GameProjects/MGE/resources"
#else
    #define RESOURCE_DIR "resources"
#endif

/// Inline macro
#define ANT_FORCE_INLINE _forceinline
#define ANT_INLINE inline

#define ANT_OVERRIDE override

namespace ant 
{
	/// Declare data types
	typedef float Real;
	typedef unsigned int UInt;
	typedef unsigned long Ulong;
	typedef double DeltaTime;
	typedef float TimeStamp;
	typedef std::vector<std::string> StringVector;

	// TODO format forward declarations
	// Forward declarations for new framework

	// Declare id for actors
	typedef UInt ActorId;

	// Declare id for actor components
	typedef UInt ComponentId;

	// Declare default values
	const ActorId INVALID_ACTOR_ID = 0;
	const ComponentId INVALID_COMPONENT_ID = 0;
	typedef UInt GameViewId;
	const GameViewId ant_InvalidGameViewId = 0xffffffff;

	//  typedef std::list<shared_ptr<IGameView> > GameViewList;

	// Declare Pointer types
	typedef unsigned long EventType;

	typedef BYTE CharCode;
}

#endif  //ANT_CORE_TYPES_HPP
