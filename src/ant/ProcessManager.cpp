#include <ant/classes/ProcessManager.hpp>
#include <ant/interfaces/IProcess.hpp>

using namespace ant;

ProcessManager::ProcessManager()
{
	// Money for nothing
}

ProcessManager::~ProcessManager( void )
{
	clearAllProcesses();
}

ant::UInt ProcessManager::updateProcesses( ant::DeltaTime dt )
{
	ant::UInt successCount = 0;
	ant::UInt failedCount = 0;

	ProcessPtrList::iterator it = m_activeProcesses.begin();
	while (it != m_activeProcesses.end())
	{
		// Grab the process
		IProcessStrongPtr pCurrentProcess = (*it);

		// increment since we might remove this process
		ProcessPtrList::iterator currentIt = it;
		++it;

		// init if not init
		if (pCurrentProcess->getState() == IProcess::UNINITIALIZED)
		{
			pCurrentProcess->onInit();
		}

		// update the process
		if (pCurrentProcess->getState() == IProcess::RUNNING)
		{
			pCurrentProcess->onUpdate(dt);
		}

		// Check if the process is dead
		if (pCurrentProcess->isDead())
		{
			// Run the exit function depending on the state of the process
			switch (pCurrentProcess->getState())
			{
				case IProcess::SUCCEEDED:
				{
					pCurrentProcess->onSuccess();
					IProcessStrongPtr pChild = pCurrentProcess->removeChild();
					if (pChild)
					{
						attachProcess(pChild);
					}
					else
					{
						++successCount; // THis apparently counts if the whole chain is done!
					}
					break;
				}				
				case IProcess::FAILED:
				{
					pCurrentProcess->onFail();
					++failedCount;
					break;
				}
				case IProcess::ABORTED:
				{
					pCurrentProcess->onAbort();
					++failedCount;
					break;
				}
			}
			// lastly remove the process form the list 
			m_activeProcesses.erase(currentIt);
		}
	}
	return ((successCount << 16) | failedCount);
}

ant::IProcessWeakPtr ProcessManager::attachProcess( IProcessStrongPtr pProcess )
{
	m_activeProcesses.push_front(pProcess);
	return IProcessWeakPtr(pProcess);
}

void ProcessManager::abortAllProcesses( bool immediate )
{
	ProcessPtrList::iterator it = m_activeProcesses.begin();
	while (it != m_activeProcesses.end())
	{
		ProcessPtrList::iterator temp = it;
		++it;

		IProcessStrongPtr p = *temp;
		if (p->isAlive())
		{
			p->setState(IProcess::ABORTED);
			if (immediate)
			{
				p->onAbort();
				m_activeProcesses.erase(temp);
			}
		}
	}
}

void ProcessManager::clearAllProcesses( void )
{
	m_activeProcesses.clear();
}

