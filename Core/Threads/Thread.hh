#ifndef Protium_Thread_hh_
#define Protium_Thread_hh_

#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <algorithm>

#include "Protium/Threads/ThreadingPolicy.hh"

namespace Protium{

	namespace Threads{

		/**
			\file Thread.h
			\author Kevin Wierman
			\date \today

			\brief An object-oriented thread class

			To be used as such:

			~~~~~~~~
			class MyThreadedTask : AbstractThread{
				...Some Fields...
			public:
				virtual void start(){
					std::cout<<"In the start method of the thread"<<std::endl;
				}
				...
			};

			//Then later on in code:

			MyThreadedTask mytask;
			//Start the thread

			myTask.Create();

			~~~~~~~~
		**/

		namespace Private{
			//Use this to call up the thread via the owning object, which should 
			void* threadingFunction(void* thread);
		}

		class AbstractThread : InstanceLocked<AbstractThread, Mutex>{
			//! Holds the thread in question
			pthread_t thread;
			//! Holds the thread attributes
			pthread_attr_t* attr;

		private:
			//! Thread status indication
			enum Status{
				idle, running, exited, killed
			};

			//! This thread's status
			Status stat;

		protected:
			typedef InstanceLocked< AbstractThread, Mutex > ThreadModel;
			typedef typename ThreadModel::Lock lock;
		public:
			//! Starts this thread from the parent thread. Needs to be overriden for root thread
			virtual void Create(){
				lock l(this);
				(void)l;
				pthread_create(&thread,attr,::Protium::Threads::Private::threadingFunction,this);				
			}

		protected:

			//! Calls exit on the current thread
			void Exit(){
				pthread_exit (&stat);
			}

		public:

			//! Calls cancel on this thread from another thread
			void Cancel(){
				pthread_cancel (thread);				
			}

		protected:

			//! Initiates this thread's attributes
			void Init(){
				pthread_attr_init (attr);				
			}
			//! De-initializes this thread's attributes. Should only be called in conjunction with Init();
			void Kill(){
				pthread_attr_destroy(attr);
			}
		public:

			//Override this for a threading routine
			virtual void start()=0;

			//Call this to join with the child thread. Blocks calling thread until the child thread joins.
			virtual void Join(const AbstractThread& other){
				//pthread_join(other.thread, &status);
			}

			static pid_t thisThreadID() {
				return pthread_self();
			}

			//bool equal(pid_t& other){
			//	return  (this->pid, other);
			//}

			void sleep_for(unsigned time){
				usleep(time);
			}

		};

		class RootThread : public AbstractThread {
			pid_t fThreadID;
			static RootThread* fRoot;
			RootThread() {
				std::atexit(&Protium::Threads::RootThread::FinishThreads);
				fThreadID = thisThreadID();
			}
			~RootThread(){

			}
		public:
			void start(){}
			static void FinishThreads(){
				delete fRoot;
				fRoot=NULL;
			}
		};

		class Thread : public AbstractThread {
		private:

			//The children of this node
			std::vector<Thread> fChildren;
			typedef std::vector<Thread>::iterator ThreadIt;
			const Thread* fParent;//Points to the parent node. Does not own
			//now for the root thread

			

			//static Thread GetRootThread(){
				//Since the root thread will always be instantiated in the main thread, we can assume the 
				//if(!fRoot)
				//	fRoot = new RootThread();
				//return fRoot;
			//}
			//this
			Thread(const Thread* parent) : fParent(parent){
				Init();
			}
		public:
			virtual ~Thread(){
				AbstractThread::Kill();
				for(ThreadIt it = fChildren.begin(); it!= fChildren.end(); ++it){
					//this->Join(*it);
				}
			}
			const AbstractThread* getParent(){
				return fParent;
			}
			template<class T>
			const T* Create(){
				T* temp = new T();
				this->fChildren.push_back(*temp);
				return temp;
			}
			//create is used to get the thread for the parent object to hold onto, now for the wierd part
			
			const Thread* Get(pid_t id){
				return NULL;
			}

			void Kill(pid_t id){
				for(ThreadIt it = fChildren.begin(); it!= fChildren.end(); ++it){
						this->Join(*it);
					}
			}
		public:
			void start(){}
		};





			










			//pthread_attr_getstacksize (attr, stacksize)

			//pthread_attr_setstacksize (attr, stacksize)

			//pthread_attr_getstackaddr (attr, stackaddr)

			//pthread_attr_setstackaddr (attr, stackaddr) 
			//void setstacksize(){
				//pthread_attr_setstacksize(/*It's over NINE THOUSAND!!!*/);
			//}


	}
}



#endif  //File Guardian

