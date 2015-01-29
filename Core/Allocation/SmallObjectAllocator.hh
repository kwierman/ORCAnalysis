#ifndef Protium_SmallObjectAllocator_hh_
#define Protium_SmallObjectAllocator_hh_

#include "Protium/Threads/ThreadingPolicy.hh"
#include "Protium/Threads/Mutex.hh"
#include "Protium/Singleton/DeletionPolicies.hh"
#include "Protium/Singleton/CreationPolicies.hh"
#include "Protium/Singleton/Singleton.hh"

#include <cstddef>

namespace Protium{

	namespace Allocation{

        class FixedAllocator;

		//! Manages a pool of fixed allocators
	    class SmallObjectAllocatorImplementation{
	    private:
	    	Protium::Allocation::FixedAllocator* fPool;
        	const std::size_t fMaxObjectSize;
        	const std::size_t fAlignSize;
    	protected:
        	SmallObjectAllocatorImplementation( std::size_t pageSize, std::size_t maxObjectSize, std::size_t objectAlignSize );
        	~SmallObjectAllocatorImplementation( void );
    	public:
        	void * Allocate( std::size_t size, bool doThrow );
        	void Deallocate( void * p, std::size_t size );
        	void Deallocate( void * p );
        	inline std::size_t GetMaxObjectSize() const { return fMaxObjectSize; }
        	inline std::size_t GetAlignment() const { return fAlignSize; }
        	bool TrimExcessMemory( void );
        	bool IsCorrupt( void ) const;
	    private:
    	    SmallObjectAllocatorImplementation( void );
    	    SmallObjectAllocatorImplementation( const SmallObjectAllocatorImplementation & );
    	    SmallObjectAllocatorImplementation & operator = ( const SmallObjectAllocatorImplementation & );
    };

    //! Singleton small object allocator
    template < template <class, class> class ThreadingModel = Protium::Threads::InSingleThread,
        std::size_t chunkSize = 4096,
        std::size_t maxSmallObjectSize = 256,
        std::size_t objectAlignSize = 4,
        template <class> class LifetimePolicy = Protium::Singleton::DeleteNever,
        class MutexPolicy = Protium::Threads::Mutex
    >
    class SmallObjectAllocator : public SmallObjectAllocatorImplementation {
    public:

        typedef SmallObjectAllocator< ThreadingModel, chunkSize, maxSmallObjectSize, objectAlignSize, LifetimePolicy > AllocatorModel;
        typedef ThreadingModel< AllocatorModel, MutexPolicy > ThreadModel;

        typedef Protium::Singleton::Singleton< AllocatorModel, Protium::Singleton::CreateStatic,
            LifetimePolicy, ThreadingModel > AllocatorSingleton;

        inline static SmallObjectAllocator & Instance( void )
        {
            return AllocatorSingleton::Instance();
        }

        inline SmallObjectAllocator() :
            SmallObjectAllocatorImplementation( chunkSize, maxSmallObjectSize, objectAlignSize )
            {}

        inline ~SmallObjectAllocator( void ) {}
        static void ClearExtraMemory( void ){
     	   typename ThreadModel::Lock lock;
        	(void)lock; // get rid of warning
        	Instance().TrimExcessMemory();
    	}

        static bool IsCorrupted( void )    {
     	    typename ThreadModel::Lock lock;
       	 	(void)lock; // get rid of warning
        	return Instance().IsCorrupt();
    	}

    private:
        SmallObjectAllocator( const SmallObjectAllocator & );
        SmallObjectAllocator & operator = ( const SmallObjectAllocator & );
    };

	    template
	    <
	        template <class, class> class T,
	        std::size_t C,
	        std::size_t M,
	        std::size_t O,
	        template <class> class L,
	        class X
	    >
	    inline unsigned int GetPriority(
	        SmallObjectAllocator< T, C, M, O, L, X >* )
	    {
	        return 0xFFFFFFFF;
	    }

	}
}


#endif //File Guardian