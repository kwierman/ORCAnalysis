#ifndef Protium_Locker_hh_
#define Protium_Locker_hh_

#include "Protium/Design/NonCopyable.hh"

namespace Protium{
	namespace Threads{

		/**
			\class Locker
			\brief To be used as an unnamed temporary for the intent of locking threaded classes in function calls.
			\usage In order to ensure that classes are thread safe when proxied, use the following in return statements:
			~~~~~~~~
				return Locker<Host>(fHost);
			~~~~~~~~
		**/

	    template <class Host>
		class Locker : public Protium::Design::NonCopyable {
    	    Host * fHost;
	    public:
        	Locker( const Host* p ) : fHost( const_cast< Host* >( p ) ){
            if ( fHost != 0 )
                fHost->Lock();
        	}

	        ~Locker(){
            if ( fHost != 0 )
                fHost->Unlock();
	        }

        	operator Host *(){
     	       return fHost;
        	}

	        Host * operator->(){
	            return fHost;
    	    }
    	};
	}
}

#endif //Protium_Locker_hh_