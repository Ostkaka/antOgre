#ifndef PROCESSMANAGERSINGLETON_HPP_
	#define PROCESSMANAGERSINGLETON_HPP_

#include <ant/core_types.hpp>

namespace ant
{
	// Forward declaration
	class ProcessManager;

	/**
	 * Class that wraps the main process manager into a singleton. Might not be a long term solution
	 */
	class ProcessManagerSingleton : Singleton
	{
	public:

		/// Creates an instance of the ResourceCacheManager
		static void startUp(void);

		/// Destroys the instance and all resource caches inside
		static void shutDown(void);

		/// Get instance
		static ProcessManagerSingleton* instance(void);

	    /// Returns the cache used by the manager
		ProcessManager* getProcessManager(void);

	protected:
		ProcessManagerSingleton();
		virtual ~ProcessManagerSingleton() {}

	//////////////////////////////////////////////////////////////////////////
	// Variables
	//////////////////////////////////////////////////////////////////////////
	protected:
		ProcessManager                 *m_processManager;
		static ProcessManagerSingleton* s_instance;
	};
}

#endif