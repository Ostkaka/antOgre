#ifndef ISINGLETON_HPP_
	#define ISINGLETON_HPP_

namespace ant
{
	class ISingleton
	{
		/// Should be called to construct the singleton
		virtual void startup() = 0;

		/// Should be called to destory the singleton
		virtual void shutdown() = 0;
		
	protected:
		virtual ~ISingleton();                      // Don't implement
		ISingleton();			                    // Don't implement
		ISingleton(ISingleton const&);              // Don't Implement.
		void operator=(ISingleton const&);          // Don't implement
	};
}

#endif