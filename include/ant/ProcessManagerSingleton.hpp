#ifndef PROCESSMANAGERSINGLETON_HPP_
	#define PROCESSMANAGERSINGLETON_HPP_

#include <ant/core_types.hpp>

namespace ant
{
	/**
	 * Class that wraps the main process manager into a singleton. Might not be a long term solution
	 */
	class ProcessManagerSingleton
	{
	public:

		/// Creates an instance of the ResourceCacheManager
		static void create(void);

		/// Destroys the instance and all resource caches inside
		static void destroy(void);

		/// Get instance
		static ProcessManagerSingleton* instance(void);

		/// Init the singleton and create internal process manager
		void init(void);

		/// Returns the cache used by the manager
		ProcessManager* getProcessManager(void);

	protected:

	private:
		// Clear all ProcessManagers
		void clearProcessManager(void);

		explicit ProcessManagerSingleton();
		~ProcessManagerSingleton();

	//////////////////////////////////////////////////////////////////////////
	// Variables
	//////////////////////////////////////////////////////////////////////////
	protected:

		ProcessManager *m_processManager;

		static ProcessManagerSingleton* s_instance;

	};
}

#endif