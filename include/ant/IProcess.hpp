#ifndef IPROCESS_HPP_
	#define IPROCESS_HPP_

#include <ant/ant_std.hpp>
#include <ant/core_types.hpp>

namespace ant
{

	/*
	Interface class for handling different game processes
	*/
	class IProcess
	{
		friend class ProcessManager;

	public:

		/// Process state
		enum State
		{
			// Neither dead or alive
			UNINITIALIZED = 0, // Created nut not running
			REMOVED, // Not in the process list but not destroyed. ex. parented to another process
			// Alive
			RUNNING, // Inited and running 
			PAUSED, // Inited but paused
			SUCCEEDED, // Succeeded
			FAILED, // Failed
			ABORTED, // Aborted
		};

		/// Default Constructor
		explicit IProcess();

		/// Default Destructor
		virtual ~IProcess();

		/// Ends the process successfully
		void succeedAndStop();

		/// Ends the process as a failure
		void failAndStop();

		void pause();

		void unPause();

		State getState() const;

		bool isAlive() const; 

		bool isDead() const;

		bool isRemoved() const;

		bool isPaused() const;

		/// Attaches a child process to the process
		void attachChild(IProcessStrongPtr pChild);
		
		/// Removes the child process from the parent process and returns a strong pointer to it
		IProcessStrongPtr removeChild(void);

		/// Returns the child of the process as a strong pointer
		IProcessWeakPtr peekChild(void);

	protected:

		virtual void onInit();

		virtual void onUpdate(ant::DeltaTime dt) = 0;

		virtual void onSuccess(){};

		virtual void onFail(){};

		virtual void onAbort(){};

	private:

		void setState(State state);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	private:
		/// Current state of the process
		State m_state; 
		/// Holds a child process if there are any
		IProcessStrongPtr m_pChild; 

	};

	// Inline implementation

	ANT_INLINE void IProcess::onInit() { m_state = RUNNING;}

  ANT_INLINE void IProcess::succeedAndStop() {
		GCC_ASSERT(isAlive());
		m_state = SUCCEEDED;
	}

	ANT_INLINE void IProcess::failAndStop() {
		GCC_ASSERT(isAlive());
		m_state = FAILED;
	}

	ANT_INLINE void IProcess::pause()
	{
		if (m_state == RUNNING)
			m_state = PAUSED;
		else
			GCC_WARNING("Attempt to pause a process that is not running! Behave yourself!");
	}

	ANT_INLINE void IProcess::unPause()
	{
		if (m_state == PAUSED)
			m_state = RUNNING;
		else
			GCC_WARNING("Attempt to unpause a process that is not paused! Behave yourself!");
	}
	
	ANT_INLINE bool IProcess::isAlive() const{ return m_state == RUNNING || m_state == PAUSED;}

	ANT_INLINE bool IProcess::isDead() const{ return m_state == SUCCEEDED || m_state == FAILED || m_state == ABORTED;}

	ANT_INLINE bool IProcess::isRemoved() const{ return m_state == REMOVED;}

	ANT_INLINE bool IProcess::isPaused() const{ return m_state == PAUSED;}

	ANT_INLINE void IProcess::setState(State state) { m_state = state;}

	ANT_INLINE IProcess::State IProcess::getState() const{ return m_state;}

	ANT_INLINE IProcessWeakPtr IProcess::peekChild() { return m_pChild;}

}

#endif