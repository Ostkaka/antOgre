#ifndef ISCRIPTMANAGER_HPP_
	#define ISCRIPTMANAGER_HPP_

namespace ant
{
	/**
	* This is the basic interface that all script manager system
	*/
	class IScriptManager
	{

	public:

		virtual ~IScriptManager(void) {}

		virtual bool init(void) = 0;

		virtual void executeFile( const char* path ) = 0;

		virtual void executeString( const char* str ) = 0;
	};
}

#endif /*ISCRIPTMANAGER_HPP_*/