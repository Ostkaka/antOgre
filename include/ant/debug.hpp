#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <ant/Logger.hpp>

//---------------------------------------------------------------------------------------------------------------------
// Debug macros
//---------------------------------------------------------------------------------------------------------------------

// Fatal Errors are fatal and are always presented to the user.
#define ANT_FATAL(str) \
do \
	{ \
	static Logger::ErrorMessenger* pErrorMessenger = ANT_NEW Logger::ErrorMessenger; \
	std::string s((str)); \
	pErrorMessenger->Show(s, true, __FUNCTION__, __FILE__, __LINE__); \
	} \
while (0)\

#if 1 //ndef NDEBUG

// Errors are bad and potentially fatal.  They are presented as a dialog with Abort, Retry, and Ignore.  Abort will
// break into the debugger, retry will continue the game, and ignore will continue the game and ignore every subsequent 
// call to this specific error.  They are ignored completely in release mode.
#define ANT_ERROR(str) \
do \
	{ \
	static Logger::ErrorMessenger* pErrorMessenger = ANT_NEW Logger::ErrorMessenger; \
	std::string s((str)); \
	pErrorMessenger->Show(s, false, __FUNCTION__, __FILE__, __LINE__); \
	} \
while (0)\

// Warnings are recoverable.  They are just logs with the "WARNING" tag that displays calling information.  The flags
// are initially set to WARNINGFLAG_DEFAULT (defined in debugger.cpp), but they can be overridden normally.
#define ANT_WARNING(str) \
do \
	{ \
	std::string s((str)); \
	Logger::Log("WARNING", s, __FUNCTION__, __FILE__, __LINE__); \
	}\
while (0)\

// This is just a convenient macro for logging if you don't feel like dealing with tags.  It calls Log() with a tag
// of "INFO".  The flags are initially set to LOGFLAG_DEFAULT (defined in debugger.cpp), but they can be overridden 
// normally.
#define ANT_INFO(str) \
do \
	{ \
	std::string s((str)); \
	Logger::Log("INFO", s, NULL, NULL, 0); \
	} \
while (0) \

// This macro is used for logging and should be the preferred method of "printf debugging".  You can use any tag 
// string you want, just make sure to enabled the ones you want somewhere in your initialization.
#define ANT_LOG(tag, str) \
do \
	{ \
	std::string s((str)); \
	Logger::Log(tag, s, NULL, NULL, 0); \
	} \
while (0) \

// This macro replaces ANT_ASSERT().
#define ANT_ASSERT(expr) \
do \
	{ \
if (!(expr)) \
		{ \
		static Logger::ErrorMessenger* pErrorMessenger = ANT_NEW Logger::ErrorMessenger; \
		pErrorMessenger->Show(#expr, false, __FUNCTION__, __FILE__, __LINE__); \
		} \
	} \
while (0) \

#else  // NDEBUG is defined

// These are the release mode definitions for the macros above.  They are all defined in such a way as to be 
// ignored completely by the compiler.
#define ANT_ERROR(str) do { (void)sizeof(str); } while(0) 
#define ANT_WARNING(str) do { (void)sizeof(str); } while(0) 
#define ANT_INFO(str) do { (void)sizeof(str); } while(0) 
#define ANT_LOG(tag, str) do { (void)sizeof(tag); (void)sizeof(str); } while(0) 
#define ANT_ASSERT(expr) do { (void)sizeof(expr); } while(0) 

#endif  // !defined NDEBUG


#endif /*DEBUG_HPP_*/