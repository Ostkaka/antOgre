#ifndef PROCESSMANAGER_H_
	#define PROCESSMANAGER_H_

#include <ant/core_types.hpp>
#include <ant/ant_std.hpp>

namespace ant 
{

	/*
	* This class manages the processes in the framework
	*/
	class ProcessManager
	{
		typedef std::list<IProcessStrongPtr> ProcessPtrList;

	public:

		ProcessManager();

		/// Default destructor
		~ProcessManager(void);

		/* 
		Update the processes in the process manager. This function returns the number of process chains that 
		succeeded in the upper 32 bits and the number of process chains that failed or were aborted in the lower 32 bits.
		*/
		ant::UInt updateProcesses(ant::DeltaTime dt);

		/// Attaches a process to the process manager
		IProcessWeakPtr attachProcess(IProcessStrongPtr pProcess);

		/// Aborts all processes currently in the manager
		void abortAllProcesses(bool immediate);

		/// Returns the number of processes attached to the manager
		ant::UInt getNumProcesses(void) const;

	private:

		/// This function is called by the destructor which clears all processes
		void clearAllProcesses(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////

	protected:
		/// List of all the active processes in the manager
		ProcessPtrList m_activeProcesses;

	};

	ANT_INLINE ant::UInt ProcessManager::getNumProcesses() const { return (unsigned int)(m_activeProcesses.size()); }

}

#endif /*PROCESSMANAGER_H_*/