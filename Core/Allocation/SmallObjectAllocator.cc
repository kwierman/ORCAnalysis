#include "Protium/Allocation/SmallObjectAllocator.hh"
#include "Protium//Allocation/FixedAllocator.hh"

inline std::size_t GetOffset( std::size_t numBytes, std::size_t alignment ){
    return ( numBytes + alignment-1 ) / alignment;
}

void * DefaultAllocator( std::size_t numBytes, bool doThrow ){
    return doThrow ? ::operator new( numBytes ) : ::operator new( numBytes, std::nothrow_t() );
}

void DefaultDeallocator( void * p ){
    ::operator delete( p );
}

Protium::Allocation::SmallObjectAllocatorImplementation::SmallObjectAllocatorImplementation( std::size_t pageSize,
    std::size_t maxObjectSize, std::size_t objectAlignSize ) :
    fPool( NULL ),
    fMaxObjectSize( maxObjectSize ),
    fAlignSize( objectAlignSize ){

    assert( 0 != objectAlignSize );
    const std::size_t allocCount = GetOffset( maxObjectSize, objectAlignSize );
    fPool = new FixedAllocator[ allocCount ];
    for ( std::size_t i = 0; i < allocCount; ++i )
        fPool[ i ].Initialize( ( i+1 ) * objectAlignSize, pageSize );
}

Protium::Allocation::SmallObjectAllocatorImplementation::~SmallObjectAllocatorImplementation(){
    delete [] fPool;
}

bool Protium::Allocation::SmallObjectAllocatorImplementation::TrimExcessMemory( void ){
    bool found = false;
    const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
    std::size_t i = 0;
    for ( ; i < allocCount; ++i )
    	found or_eq fPool[i].TrimEmptyChunk();
    for ( i = 0; i < allocCount; ++i )
    	found or_eq fPool[i].TrimChunkList();
    return found;
}

void* Protium::Allocation::SmallObjectAllocatorImplementation::Allocate( std::size_t numBytes, bool doThrow ){
    if ( numBytes > GetMaxObjectSize() )
        return DefaultAllocator( numBytes, doThrow );

    assert( fPool != NULL );
    if ( numBytes == 0 ) numBytes = 1;
    const std::size_t index = GetOffset( numBytes, GetAlignment() ) - 1;
    const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
    (void) allocCount;
    assert( allocCount > index  );

    FixedAllocator& allocator = fPool[ index ];
    assert( allocator.BlockSize() >= numBytes );
    assert( allocator.BlockSize() < numBytes + GetAlignment() );
    void* place = allocator.Allocate();

    if ( ( place == NULL ) && TrimExcessMemory() )
        place = allocator.Allocate();

    if ( ( place == NULL ) && doThrow )
        throw std::bad_alloc();
    return place;
}

void Protium::Allocation::SmallObjectAllocatorImplementation::Deallocate( void * p, std::size_t numBytes ){
    if ( p == NULL ) return;
    if ( numBytes > GetMaxObjectSize() ){
        DefaultDeallocator( p );
        return;
    }

    assert( fPool != NULL );
    if ( numBytes ==0 ) numBytes = 1;
    const std::size_t index = GetOffset( numBytes, GetAlignment() ) - 1;
    const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
    (void) allocCount;
    assert( index < allocCount );
    FixedAllocator & allocator = fPool[ index ];
    assert( allocator.BlockSize() >= numBytes );
    assert( allocator.BlockSize() < numBytes + GetAlignment() );
    const bool found = allocator.Deallocate( p, NULL );
    (void) found;
    assert( found );
}

void Protium::Allocation::SmallObjectAllocatorImplementation::Deallocate( void * p ){
    if ( p == NULL ) return;
    assert(  fPool != NULL );
    const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
    Chunk * chunk = NULL;
    FixedAllocator * pAllocator = NULL;

    for ( std::size_t ii = 0; ii < allocCount; ++ii ) {
        chunk = fPool[ ii ].HasBlock( p );
        if ( chunk != NULL ){
            pAllocator = &fPool[ ii ];
            break;
        }
    }
    if ( pAllocator == NULL ){
        DefaultDeallocator( p );
        return;
    }

    assert( NULL != chunk );
    const bool found = pAllocator->Deallocate( p, chunk );
    (void) found;
    assert( found );
}


bool Protium::Allocation::SmallObjectAllocatorImplementation::IsCorrupt( void ) const{
    if ( fPool == NULL ){
        assert( false );
        return true;
    }
    if ( 0 == GetAlignment() ) {
        assert( false );
        return true;
    }
    if ( 0 == GetMaxObjectSize() ) {
        assert( false );
        return true;
    }
    const std::size_t allocCount = GetOffset( GetMaxObjectSize(), GetAlignment() );
    for ( std::size_t ii = 0; ii < allocCount; ++ii ){
        if ( fPool[ ii ].IsCorrupt() )
            return true;
    }
    return false;
}