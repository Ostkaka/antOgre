#include <ant/interfaces/IProcess.hpp>

using namespace ant;


ant::IProcess::IProcess()
	:m_state(UNINITIALIZED)
{
}

ant::IProcess::~IProcess()
{
	if (m_pChild)
	{
		m_pChild->onAbort();
	}
}

void ant::IProcess::attachChild( IProcessStrongPtr pChild )
{
	if (m_pChild)
	{
		m_pChild->attachChild(pChild);
	}
	else
	{
		m_pChild = pChild;
	}		
}

ant::IProcessStrongPtr ant::IProcess::removeChild( void )
{
	if (m_pChild)
	{
		IProcessStrongPtr pChild = m_pChild;  
		m_pChild.reset();		
		return pChild;
	}
	return IProcessStrongPtr();
}

