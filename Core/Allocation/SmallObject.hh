
#ifndef Protium_SmallObject_hh_
#define Protium_SmallObject_hh_

#include "Protium/Allocation/SmallObjectAllocator.hh"
#include "Protium/Singleton/DeletionPolicies.hh"
#include "Protium/Threads/ThreadingPolicy.hh"


namespace Protium{

    namespace Allocation{

        /*! \class SmallObjectBase
            \brief Base class for small objects to be used in the small object allocation scheme 
            \param ThreadPolicy Include threading policy here from ThreadingPolicy.h
            \param chunkSize defines the size of chunks
            \param maxSmallObjectSize objects exceeding this size will default to standard alloction (new or malloc)
            \param objectAlignSize used by allocator for default object alignment
            \param LifetimePolicy Determines the lifetime of the allocator singleton from DeletionPolicies.h
            \param MutexPolicy Determines the mutex type to be used. (see Mutex.h)

            Use as a base class for small objects in order to utilize the SmallObjectAllocator.

        */
        template< template <class, class> class ThreadPolicy,
            std::size_t chunkSize,
            std::size_t maxSmallObjectSize,
            std::size_t objectAlignSize,
            template <class> class LifetimePolicy,
            class MutexPolicy>
        class SmallObjectBase{
        public:
            //! Type of allocator to host the allocator singleton
            typedef Protium::Allocation::SmallObjectAllocator< ThreadPolicy, chunkSize,
                maxSmallObjectSize, objectAlignSize, LifetimePolicy > ObjAllocator;
        
        private:

            typedef ThreadPolicy< ObjAllocator, MutexPolicy > ThreadType;
            typedef typename ObjAllocator::AllocatorSingleton AllocatorSingleton;
            
        public:

            static void * operator new ( std::size_t size ) throw ( std::bad_alloc ){
                typename ThreadType::Lock lock;
                (void)lock;
                return AllocatorSingleton::Instance().Allocate( size, true );
            }

            static void * operator new ( std::size_t size, const std::nothrow_t & ) throw () {
                typename ThreadType::Lock lock;
                (void)lock;
                return AllocatorSingleton::Instance().Allocate( size, false );
            }

            inline static void * operator new ( std::size_t size, void * place ) {
                return ::operator new( size, place );
            }

            static void operator delete ( void * p, std::size_t size ) throw () {
                typename ThreadType::Lock lock;
                (void)lock;
                AllocatorSingleton::Instance().Deallocate( p, size );
            }

            static void operator delete ( void * p, const std::nothrow_t & ) throw() {
                typename ThreadType::Lock lock;
                (void)lock; // get rid of warning
                AllocatorSingleton::Instance().Deallocate( p );
            }

            inline static void operator delete ( void * p, void * place ) {
                ::operator delete ( p, place );
            }

            static void * operator new [] ( std::size_t size ) throw ( std::bad_alloc ) {
                typename ThreadType::Lock lock;
                (void)lock; // get rid of warning
                return AllocatorSingleton::Instance().Allocate( size, true );
            }

            static void * operator new [] ( std::size_t size, const std::nothrow_t & ) throw () {
                typename ThreadType::Lock lock;
                (void)lock; // get rid of warning
                return AllocatorSingleton::Instance().Allocate( size, false );
            }

            inline static void * operator new [] ( std::size_t size, void * place ) {
                return ::operator new( size, place );
            }

            static void operator delete [] ( void * p, std::size_t size ) throw () {
                typename ThreadType::Lock lock;
                (void)lock; // get rid of warning
                AllocatorSingleton::Instance().Deallocate( p, size );
            }

            static void operator delete [] ( void * p, const std::nothrow_t & ) throw(){
                typename ThreadType::Lock lock;
                (void)lock; // get rid of warning
                AllocatorSingleton::Instance().Deallocate( p );
            }

            inline static void operator delete [] ( void * p, void * place ) {
                ::operator delete ( p, place );
            }

        protected:
            inline SmallObjectBase( void ) {}
            inline SmallObjectBase( const SmallObjectBase & ) {}
            inline SmallObjectBase & operator = ( const SmallObjectBase & )
            { return *this; }
            inline ~SmallObjectBase() {}
        };


        /*! \class SmallObject
            \brief Inherit from this in order to benefit from small object allocation.

            Base class for small objects less than 256 bytes in size. If object is a valueobject, use SmallValueObject instead.
        */
        template
        <
            template <class, class> class ThreadingModel = Protium::Threads::InSingleThread,
            std::size_t chunkSize = 4096,
            std::size_t maxSmallObjectSize = 256,
            std::size_t objectAlignSize = 4,
            template <class> class LifetimePolicy = Protium::Singleton::DeleteNever,
            class MutexPolicy =  Protium::Threads::Mutex
        >
        class SmallObject : public SmallObjectBase< ThreadingModel, chunkSize,
                maxSmallObjectSize, objectAlignSize, LifetimePolicy, MutexPolicy >
        {

        public:
            virtual ~SmallObject() {}
        protected:
            inline SmallObject( void ) {}

        private:
            SmallObject( const SmallObject & );
            SmallObject & operator = ( const SmallObject & );
        }; //end class small object


        /*! \class SmallValueObject
            \brief Inherit from this in order to benefit from small object allocation.
            

            Base class for small objects less than 256 bytes in size. If object is a valueobject, use SmallValueObject instead.
        */
        template< template <class, class> class ThreadingModel = Protium::Threads::InSingleThread,
            std::size_t chunkSize = 4096,
            std::size_t maxSmallObjectSize = 256,
            std::size_t objectAlignSize = 4,
            template <class> class LifetimePolicy = Protium::Singleton::DeleteNever,
            class MutexPolicy = Protium::Threads::Mutex >
        class SmallValueObject : public SmallObjectBase< ThreadingModel, chunkSize,
                maxSmallObjectSize, objectAlignSize, LifetimePolicy, MutexPolicy >
        {
        protected:
            inline SmallValueObject( void ) {}
            inline SmallValueObject( const SmallValueObject & ) {}
            inline SmallValueObject & operator = ( const SmallValueObject & )
            { return *this; }
            inline ~SmallValueObject() {}
        }; // end class SmallValueObject

        //! A default small obect type.
        /*!
            \warning Do not use if you are using as a base class for a singleton object
        */
        typedef SmallObject<> DefaultSmallObject;
        typedef SmallValueObject<> DefaultSmallValueObject;

    }
}

#include "Protium/Allocation/STLAdapter.hh"

#endif //Protium_SmallObject_h_