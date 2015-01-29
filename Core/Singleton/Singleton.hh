#ifndef Protium_Singleton_hh_
#define Protium_Singleton_hh_

#include "Protium/Singleton/CreationPolicies.hh"
#include "Protium/Singleton/DeletionPolicies.hh"
#include "Protium/Threads/ThreadingPolicy.hh"
#include "Protium/Threads/Mutex.hh"

namespace Protium{

    /** Contains all helper objects for creating threadsafe singletons with longevity tracking
    **/
    namespace Singleton{

        /** Holder class for singleton instances.
            \param Host A predefined class to be singleton'd
            \param CreationPolicy How the instance should be created. (see CreationPolicies.h)
            \param DeletionPolicy How the instance should be deleted. (see DeletionPolicies.h)
            \param ThreadingPolicy Defines the level of locking in the class
            \param MutexPolicy Defined the mutex-ability of the class
        **/
        template<   class Host,
                    template <class> class CreationPolicy = CreateNew,
                    template <class> class DeletionPolicy = DeleteOnce,
                    template <class, class> class ThreadingPolicy = Protium::Threads::InstanceLocked,
                    class MutexPolicy = Protium::Threads::Mutex>
        class Singleton{
        public:
            //! Declaration of the host interior to the class
            typedef Host ObjectType;
            //! Use to get instance 
            static Host& Instance();
            
        private:
            //! Helper Function to make instance (Uses CreationPolicy)
            static void MakeInstance();
            //! Helper Function to destroy instance (Uses Destruction policy)
            static void DestroySingleton();
            
            //! Declared Private so can not be used 
            Singleton();
            //! Declared Private so can not be used 
            Singleton(const Singleton&) {}
            //! Declared Private so can not be used 
            Singleton& operator=(const Singleton& ) {} 
            
            //! Declaration of data interior to class
            typedef typename ThreadingPolicy<ObjectType*,MutexPolicy>::VolatileType PtrInstanceType;
            //! Pointer to the interior data
            static PtrInstanceType fInstance;
            //! Flag for whether or not the instance has been killed (yet)
            static bool fDestroyed;
        };





        template<   class Host,
                    template <class> class CreationPolicy,
                    template <class> class DeletionPolicy,
                    template <class, class> class ThreadingPolicy,
                    class MutexPolicy >
        typename Singleton<Host, CreationPolicy, DeletionPolicy, ThreadingPolicy, MutexPolicy>::PtrInstanceType
            Singleton<Host, CreationPolicy, DeletionPolicy, ThreadingPolicy, MutexPolicy>::fInstance = 0;

        template<   class Host,
                    template <class> class CreationPolicy,
                    template <class> class DeletionPolicy,
                    template <class, class> class ThreadingPolicy,
                    class MutexPolicy >
       bool Singleton<Host, CreationPolicy, DeletionPolicy, ThreadingPolicy, MutexPolicy>::fDestroyed = false;

    ////////////////////////////////////////////////////////////////////////////////
    // Singleton::Instance
    ////////////////////////////////////////////////////////////////////////////////

        template<   class Host,
                    template <class> class CreationPolicy,
                    template <class> class DeletionPolicy,
                    template <class, class> class ThreadingPolicy,
                    class MutexPolicy >
        inline Host& Singleton<Host, CreationPolicy, DeletionPolicy, ThreadingPolicy, MutexPolicy>::Instance(){
            if (!fInstance)
                MakeInstance();
            return *fInstance;
        }

    ////////////////////////////////////////////////////////////////////////////////
    // Singleton::MakeInstance (helper for Instance)
    ////////////////////////////////////////////////////////////////////////////////

        template<   class Host,
                    template <class> class CreationPolicy,
                    template <class> class DeletionPolicy,
                    template <class, class> class ThreadingPolicy,
                    class MutexPolicy >
        void Singleton<Host, CreationPolicy, DeletionPolicy, ThreadingPolicy, MutexPolicy>::MakeInstance(){
        
            typename ThreadingPolicy<Singleton,MutexPolicy>::Lock lock;
        
            (void)lock;
        
            if (!fInstance){
                if (fDestroyed){
                    fDestroyed = false;
                    DeletionPolicy<Host>::OnDeadReference();
                }
                fInstance = CreationPolicy<Host>::Create();
                DeletionPolicy<Host>::ScheduleDestruction(fInstance, &DestroySingleton);
            }
        }

        template<   class Host,
                    template <class> class CreationPolicy,
                    template <class> class DeletionPolicy,
                    template <class, class> class ThreadingPolicy,
                    class MutexPolicy >
        void Singleton<Host, CreationPolicy, DeletionPolicy, ThreadingPolicy, MutexPolicy>::DestroySingleton() {
            assert(!fDestroyed);
            CreationPolicy<Host>::Destroy(fInstance);
            fInstance = NULL;
            fDestroyed = true;
        }
    }
}


#endif //File Guardian