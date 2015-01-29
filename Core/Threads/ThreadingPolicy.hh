#ifndef Protium_ThreadingPolicy_hh_
#define Protium_ThreadingPolicy_hh_

#include <cassert>

#include "Protium/Threads/Mutex.hh"


//TODO: Add in the functionality for single threaded objects as seen in Android where multiple thread access produces an error and causes parent thread to exit


namespace Protium{

	namespace Threads{

		//! Prototype of threading policy
		/**
			Use in order to specify threading policy for a class
			For instance:
			~~~~~~~~~~~~~~~~~~~~~
			template< template<class,class>class threadPolicy = SomeThreadPolicy<MyClass,Mutex> >
			class MyClass {
				typedef MyClass<threadPolicy> localClass;
				typedef SomeThreadPolicy< localClass, Mutex > ThreadModel;
				void SomeFunction(){
	     	    	typename ThreadModel::Lock lock;
        			(void)lock; // get rid of warning
        			//Do Something thread-safe here
				}
			};
			~~~~~~~~~~~~~~~~~~~~~
			
			Now, you can specify what kind of threaded object you want to use

			~~~~~~~~~~~~~~~~~~~~~
			typedef MyClass<InSingleThread<MyClass, Mutex> > MyThreadedClass;
			~~~~~~~~~~~~~~~~~~~~~

			Is you specifically want something to be instance locked, then it has to inherit from the threading policy.

			Like so:
			~~~~~~~~~~~~~~~~~~~~~
			template< template<class,class>class threadPolicy = InstanceLocked<MyClass,Mutex> >
			class MyClass : InstanceLocked<MyClass, Mutex> {
				typedef MyClass<threadPolicy> localClass;
				typedef SomeThreadPolicy< localClass, Mutex > ThreadModel;
				void SomeFunction(){
	     	    	typename ThreadModel::Lock lock(*this);
        			(void)lock; // get rid of warning
        			//Do Something thread-safe here
				}
			};
			~~~~~~~~~~~~~~~~~~~~~


		**/
		template<class Host>
		class ThreadingPrototype{
		public:
			//! Use to access Host class from derived templates
			typedef Host VolatileType;
		};

		//! Use to specify non-thread safe classes
		template<class Host, class MutexPolicy=Mutex>
		class InSingleThread : public ThreadingPrototype<Host> {
		public:
			//! Dummy internal class which locks nothing
			class Lock{
			public:
				//! On creation, does nothing
				Lock(){}
				//! For instance locking
				explicit Lock(const InSingleThread& ){}
				//! For static locking
				explicit Lock(const InSingleThread* ){}
			};
		};

		//! Use to specify instance-locked items
		template<class Host, class MutexPolicy=Mutex>
		class InstanceLocked : public ThreadingPrototype<Host>{
			//! Contains the mutex used to lock this class
			mutable MutexPolicy fMtx;
		public:
			//! Defaults to Mutex being unlocked
	        InstanceLocked() : fMtx() {}
	        //! On copy, the copy instance is unlocked
    	    InstanceLocked(const InstanceLocked&) : fMtx() {}
    	    //! Destruction unlocks class
        	~InstanceLocked() {}

        	//! Internal class which locks the host whenever invoked
        	class Lock;

        	//! Lock is friended to the host
        	friend class Lock;

	        class Lock{
	        	//! Reference to hosted class
	        	const InstanceLocked& fHost;
	        	//! No default Locking
	        	Lock(){}
	        	//! No Copy locking
	        	Lock(const Lock&){}

	        	//! No Assignment locking
	        	Lock& operator=(const Lock&){}
	        public:
	        	//! Lock on host when constructed
	            explicit Lock(const InstanceLocked& host) : fHost(host){
	                fHost.fMtx.Lock();
	            }

	            //! Lock on host pointer when constructed
	            explicit Lock(const InstanceLocked* host) : fHost(*host){
	                fHost.fMtx.Lock();
	            }

	            //! Unlock on destruction
	            ~Lock(){
	                fHost.fMtx.Unlock();
	            }
	        };

		};

		//! Use to specify objects which lock by class
		/**
			For instance, to prevent bad file read-writes, all streams should lock simultaniously by double-lock single check.
		**/
		template<class Host, class MutexPolicy=Mutex>
		class StaticLocked : public ThreadingPrototype<Host>{

			//! Flag-variable class which is used to statically store a mutex.
	        static struct Initializer{
	        	
	        	//! Flag variable to be used in case lock is instantiated mid-initialization
	            bool fIsInit;

	            //! Static mutex for all instances
	            MutexPolicy fMtx;

	            //! On construction creates the mutex
	            Initializer() : fIsInit(false), fMtx()
	            {
	                fIsInit = true;
	            }
	            //! makes it impossible to destroy the initializer before it's created
	            ~Initializer()
	            {
	                assert(fIsInit);
	            }
	        } fInitializer;

	    public:
	    	class Lock;
        	friend class Lock;

        	//! Lock on instance creation uses the static initializer to lock all instances of host
        	class Lock{
	        public:

    	        Lock(){
            	    assert(fInitializer.fIsInit);
                	fInitializer.fMtx.Lock();
            	}

            	//! For COmpatibility with instance locked
            	explicit Lock(const StaticLocked&){
                	assert(fInitializer.fIsInit);
                	fInitializer.fMtx.Lock();
            	}

			    //! For compatibility with instance locked
			    explicit Lock(const StaticLocked*)
			    {
			        assert(fInitializer.fIsInit);
			        fInitializer.fMtx.Lock();
			    }

			    //! Unlocks class
			    ~Lock(){
			        assert(fInitializer.fIsInit);
			        fInitializer.fMtx.Unlock();
			    }

			private:
			    Lock(const Lock&);
			    Lock& operator=(const Lock&);
			};

		};

		template < class Host, class MutexPolicy >
    	typename StaticLocked< Host, MutexPolicy >::Initializer
    	StaticLocked< Host, MutexPolicy >::fInitializer;
	}

}

#endif //File Guardian