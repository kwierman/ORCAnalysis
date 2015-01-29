#ifndef Protium_SignalCatching_hh_
#define Protium_SignalCatching_hh_ 

#include "Protium/Singleton/Singleton.hh"
#include "Protium/Threads/ThreadingPolicy.hh"
#include "Protium/Threads/Mutex.hh"

#include <signal.h>

#include <iostream>

namespace Protium{

	namespace Private{
		void signal_callback_handler(int signum);
		static inline void print_stacktrace(unsigned int stack_depth = 100);

	}

	namespace Utilities{


		//!Abstract Class to use as a handler for exceptions passed to the SignalCatching Singleton
		template<int i>
		class SignalHandler{
		public:
			static bool Handles(const int& signum){return signum==i;}
			virtual ~SignalHandler(){}
			virtual void Handle()=0;
		};
		//! Handles SigInt Signals
		typedef SignalHandler<SIGINT>  SigIntHandler ;
		//! Handles SigAbrt handles (doesn't actually, but for completeness)
		typedef SignalHandler<SIGABRT> SigAbrtHandler ;
		//! Handles SigBus Errors
		typedef SignalHandler<SIGBUS>  SigBusHandler ;
		//! Handles SigSegv signals (for cleanup purposes, not to be used for graceful recovery)
		typedef SignalHandler<SIGSEGV> SigSegvHandler ;

		//TODO: Implement Signal Raising scheme here.

		class SignalCatchingImplementation{

			public: 
				SignalCatchingImplementation(){}
				virtual ~SignalCatchingImplementation(){}
			public:
				void Setup(){
					signal(SIGINT, Protium::Private::signal_callback_handler);
					signal(SIGABRT, Protium::Private::signal_callback_handler);
					signal(SIGBUS , Protium::Private::signal_callback_handler);
					signal(SIGSEGV , Protium::Private::signal_callback_handler);
				}
				//TODO:: Generalize this so that this handles a stack of handlers. 
					//Any one may interrupt the handler and proceed with calculations
				bool Handle(int signum){
					std::cout<<std::endl<<"Handling Signal: "<<signum<<std::endl;
				}
			private:
	    	    SignalCatchingImplementation( const SignalCatchingImplementation & );
	    	    SignalCatchingImplementation & operator = ( const SignalCatchingImplementation & );
		};

		typedef Protium::Singleton::Singleton<SignalCatchingImplementation, 
		Protium::Singleton::CreateNew, 
		Protium::Singleton::DeleteOnce, 
		Protium::Threads::StaticLocked> SignalCatcher;

	}
}


#endif //Protium_SignalCatching_h_

