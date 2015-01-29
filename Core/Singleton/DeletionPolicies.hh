#ifndef Protium_DeletionPolicies_hh_
#define Protium_DeletionPolicies_hh_

#include <list>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <iostream>

namespace Protium{

	namespace Singleton{

        //! Defines the type of functions to be called at exit
        typedef void (*exit_function_pointer_type)();

        //! Gets added to the std::atexit queue every time a singleton get created. Called once for every singleton at end of execution. Deletes singletons in order.
        void AtExitFn();

		//! Helper struct for deleting classes
		//! \param T The class to be deleted
        template <typename T>
        struct Deleter{
        	//! Convenience typedef
            typedef void (*Type)(T*);
            //! Default deletion method
            static void Delete(T* pObj)
            { delete pObj; }
        };

        //! Deletion Adapter class
        template <class T>
        struct Adapter{
            //! The Function to be called when the object is deleted
            exit_function_pointer_type fFun;
            //! To be used as a callback
            void operator()(T*) { return fFun(); }
        };


        //! Abstract class to hold priority for deletion
        class DeletionTracker{
            //! Priority of this class to be deleted (higher implies greater priority)
            unsigned int fPriority;
        public:
            //! Default constructor
            DeletionTracker(unsigned int x) : fPriority(x) {}

            //! Abstract definition            
            virtual ~DeletionTracker() = 0;
            
            //! For Comparing priority of deletion
            static bool Compare(const DeletionTracker* lhs, const DeletionTracker* rhs){
                return lhs->fPriority > rhs->fPriority;
            }
        };

        //! Definition Required Here
        inline DeletionTracker::~DeletionTracker(){}

        //! Queue for deletion. Elements may be added, subtracted, and at the end of exection finalized.
        static std::list<DeletionTracker*>* fTrackerList=0;

        //! Concrete templated instance of a deletion tracker
        //! \param Host Type of object to be tracked
        //! \param Destroyer Templated type of destroyer (usually derivative of Deleter)
        template <typename Host, typename Destroyer>
        class ConcreteDeletionTracker : public DeletionTracker{
            //! Pointer to the host object
            Host* fHost;
            //! instance of destroyer
            Destroyer fDestroyer;

        public:
            //! Default constructor
            //! \param p The host object to be deleted
            //! \param priority the priority of the object to be deleted (higher gets deleted first)
            //! \param d the destroyer to be used
            ConcreteDeletionTracker(Host* p , unsigned int priority, Destroyer d)
                : DeletionTracker(priority)
                , fHost(p)
                , fDestroyer(d)
            {}
            
            //! Calls the deletion method 
            ~ConcreteDeletionTracker()
            { fDestroyer(fHost); }

        };

        //! Helper function for putting an object into the deletion queue
        template <typename Host, typename Destroyer>
        void SetPriority(Host* pDynObject, unsigned int priority, Destroyer d){
            if(fTrackerList==0){
                fTrackerList = new std::list<DeletionTracker*>;
            }

            DeletionTracker* p  = new ConcreteDeletionTracker<Host, Destroyer>(pDynObject, priority, d);

            std::list<DeletionTracker*>::iterator pos = std::upper_bound( fTrackerList->begin(), fTrackerList->end(), p, DeletionTracker::Compare );
            fTrackerList->insert(pos, p);
            std::atexit(AtExitFn);
            std::cout<<"Adding in new Tracker with list size: "<<fTrackerList->size()<<std::endl;
        }

        //! Helper function for setting singleton deletion priority
        template <typename Host>
        void SetPriority(Host* pDynObject, unsigned int priority,typename Deleter< Host >::Type d = Deleter< Host >::Delete ){
           SetPriority<Host, typename Deleter< Host >::Type >(pDynObject, priority, d);
        }

        template <class Host>
        struct DeleteNever{
            static void ScheduleDestruction(Host*, exit_function_pointer_type){}
            static void OnDeadReference(){}
        };

        template <class Host>
        struct DeleteOnce{
            static void ScheduleDestruction(Host*, exit_function_pointer_type pFun) { std::atexit(pFun); }
            static void OnDeadReference() { throw std::logic_error("Dead Singleton Reference Detected"); }
        };


        template <class Host>
        class DeleteTwice{
            static bool fDestroyed;
        public:
            static void ScheduleDestruction(Host*, exit_function_pointer_type pFun){
                if(! fDestroyed ) std::atexit(pFun);
            }
            static void OnDeadReference(){
                fDestroyed=true;
            }
        };

        template<class Host>
        bool DeleteTwice<Host>::fDestroyed = false;
    
        template <class Host>
        class DeleteRandom{
        protected:
            static bool fDead;
            static bool fNeedsCallback;
            static exit_function_pointer_type deleter;
        public:
            static void ScheduleDestruction(Host*, exit_function_pointer_type pFun){
                static bool firstTime = true;
                fDead = false;
                deleter = pFun;
                if(firstTime || fNeedsCallback){
                    std::atexit(atexitCallback);
                    firstTime=false;
                    fNeedsCallback = false;
                }

            }
            static void OnDeadReference(){}

            static void GracefulDelete(){
                if(fDead) return;
                fDead = true;
                deleter();
            }
    
            static void atexitCallback(){
                fNeedsCallback = true;
                GracefulDelete();

            }
        };

        template <class Host>
        exit_function_pointer_type DeleteRandom<Host>::deleter = NULL;
    
        template <class Host>
        bool DeleteRandom<Host>::fDead = true;
    
        template <class Host>
        bool DeleteRandom<Host>::fNeedsCallback = true;


        template <class Host>
        class DeleteWithPriority{
        public:
            static void ScheduleDestruction(Host* pObj, exit_function_pointer_type pFun){
        
            Adapter<Host> adapter = { pFun };
            SetPriority(pObj, GetLongevity(pObj) , adapter);
            }
            static void OnDeadReference() { throw std::logic_error("Dead Reference Detected"); }
        };

        template <unsigned int priority, class Host>
        class DeleteWithFixedPriority{
        public:
            virtual ~DeleteWithFixedPriority(){}
            
            static void ScheduleDestruction(Host* pObj, exit_function_pointer_type pFun){
                Adapter<Host> adapter = { pFun };
                SetPriority(pObj, priority , adapter);
            }
            
            static void OnDeadReference() { throw std::logic_error("Dead Reference Detected"); }
        };

        template <class Host>
        struct DeleteLast  : DeleteWithFixedPriority<0xFFFFFFFF ,Host>{};

        template <class Host>
        struct DeleteNextToLast  : DeleteWithFixedPriority<0xFFFFFFFE ,Host>{};
 
        template <class Host>
        struct DeleteFirst : DeleteWithFixedPriority<0x0,Host>{};


        class DeleteComposite{

            template<class Host>
            class Composite{
                typedef std::vector<exit_function_pointer_type> Container;
                typedef typename Container::iterator iterator;
                static Container* fContainer;
            public:
                static void Initialize(){
                    static bool done = false;
                    if(!done){
                        fContainer = new Container;
                        done=true;
                    }
                }
                static bool Insert(exit_function_pointer_type pFun){
                    Initialize();
                    fContainer->push_back(pFun);
                }
                static bool Destruct(){
                    Initialize();
                    for(iterator it = fContainer->begin(); it!= fContainer->end(); ++it)
                        (*it)();
                    delete fContainer;
                }

            };
        public:
            template<template <class> class DestructionPolicy >
            struct DestroyWith{
                template<class Host>
                struct DestroyWithHeader{
                    static void ScheduleDestruction(Host* pObj, exit_function_pointer_type pFun){
                        Composite<Host>::Initialize();
                        DestructionPolicy<Host>::ScheduleDestruction(pObj, pFun);
                        DestructionPolicy<Host>::ScheduleDestruction(0,Composite<Host>::Destruct);
                    }
                    static void OnDeadReference() { throw std::logic_error("Dead Reference Detected"); }
                };
            };
            template<template <class> class DestructionPolicy >
            struct DestroyAfter{
                template<class Host>
                struct IsDestroyed{
                    static void ScheduleDestruction(Host* pObj, exit_function_pointer_type pFun){
                        Composite<Host>::Insert(pFun);
                    }
                    static void OnDeadReference() { throw std::logic_error("Dead Reference Detected"); }
                };
            };

        };


        template<class Host>
        typename DeleteComposite::Composite<Host>::Container* DeleteComposite::Composite<Host>::fContainer = NULL;

	}
}


#endif //File Guardian