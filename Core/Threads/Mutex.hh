/*! \file Mutex.h
    \brief Defines a very basic object-oriented mutex.
    \author Kevin Wierman
 */

#ifndef Protium_Mutex_hh_
#define Protium_Mutex_hh_

#include <pthread.h> //For access to pthreads

namespace Protium{
    namespace Threads{

        /*! \class Mutex
            \brief Wrapper class for pthreads-style mutexes
        
            Provides the ability to lock mutexes at the object level.
            Use as such:

            ~~~~~~~~
                Mutex mtx;
                mtx.Lock();
                if(mtx.IsLocked() )
                {
                    ...do some things...
                }
                mtx.Unlock();
            ~~~~~~~~

         */
        class Mutex{
            Mutex(const Mutex&);
            Mutex & operator=(const Mutex& );
            pthread_mutex_t fMtx;
            bool fIsLocked;
        public:
            Mutex();
            ~Mutex();
            //! Attempts to lock the mutex and blocks until mutex is available. 
            void Lock();
            //! Unlocks the mutex for the next usage.
            void Unlock();
            /*! Checks to see if the  mutex if locked
            *   \return True if the mutex is locked elsewhere. False otherwise.
            */
            bool IsLocked();
        };
    }
}



#endif //Protium_Threads_h_
