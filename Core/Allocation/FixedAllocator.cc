#include "Protium/Allocation/FixedAllocator.hh"

#include <assert.h>

unsigned char Protium::Allocation::FixedAllocator::MinObjectsPerChunk_ = 8;
unsigned char Protium::Allocation::FixedAllocator::MaxObjectsPerChunk_ = 255;

Protium::Allocation::FixedAllocator::FixedAllocator()
    : blockSize_( 0 )
    , numBlocks_( 0 )
    , chunks_( 0 )
    , allocChunk_( NULL )
    , deallocChunk_( NULL )
    , emptyChunk_( NULL )
{
}

Protium::Allocation::FixedAllocator::~FixedAllocator()
{
    for ( ChunkIter i( chunks_.begin() ); i != chunks_.end(); ++i )
       i->Release();
}


void Protium::Allocation::FixedAllocator::Initialize( std::size_t blockSize, std::size_t pageSize )
{
    assert( blockSize > 0 );
    assert( pageSize >= blockSize );
    blockSize_ = blockSize;

    std::size_t numBlocks = pageSize / blockSize;
    if ( numBlocks > MaxObjectsPerChunk_ ) numBlocks = MaxObjectsPerChunk_;
    else if ( numBlocks < MinObjectsPerChunk_ ) numBlocks = MinObjectsPerChunk_;

    numBlocks_ = static_cast<unsigned char>(numBlocks);
    assert(numBlocks_ == numBlocks);
}

std::size_t Protium::Allocation::FixedAllocator::CountEmptyChunks( void ) const{
    return ( NULL == emptyChunk_ ) ? 0 : 1;
}

bool Protium::Allocation::FixedAllocator::IsCorrupt( void ) const {
    const bool isEmpty = chunks_.empty();
    ChunkCIter start( chunks_.begin() );
    ChunkCIter last( chunks_.end() );
    const size_t emptyChunkCount = CountEmptyChunks();

    if ( isEmpty )
    {
        if ( start != last )
        {
            assert( false );
            return true;
        }
        if ( 0 < emptyChunkCount )
        {
            assert( false );
            return true;
        }
        if ( NULL != deallocChunk_ )
        {
            assert( false );
            return true;
        }
        if ( NULL != allocChunk_ )
        {
            assert( false );
            return true;
        }
        if ( NULL != emptyChunk_ )
        {
            assert( false );
            return true;
        }
    }

    else
    {
        const Chunk * front = &chunks_.front();
        const Chunk * back  = &chunks_.back();
        if ( start >= last )
        {
            assert( false );
            return true;
        }
        if ( back < deallocChunk_ )
        {
            assert( false );
            return true;
        }
        if ( back < allocChunk_ )
        {
            assert( false );
            return true;
        }
        if ( front > deallocChunk_ )
        {
            assert( false );
            return true;
        }
        if ( front > allocChunk_ )
        {
            assert( false );
            return true;
        }

        switch ( emptyChunkCount )
        {
            case 0:
                if ( emptyChunk_ != NULL )
                {
                    assert( false );
                    return true;
                }
                break;
            case 1:
                if ( emptyChunk_ == NULL )
                {
                    assert( false );
                    return true;
                }
                if ( back < emptyChunk_ )
                {
                    assert( false );
                    return true;
                }
                if ( front > emptyChunk_ )
                {
                    assert( false );
                    return true;
                }
                if ( !emptyChunk_->HasAvailable( numBlocks_ ) )
                {
                    // This may imply somebody tried to delete a block twice.
                    assert( false );
                    return true;
                }
                break;
            default:
                assert( false );
                return true;
        }
        for ( ChunkCIter it( start ); it != last; ++it )
        {
            const Chunk & chunk = *it;
            if ( chunk.IsCorrupt( numBlocks_, blockSize_, true ) )
                return true;
        }
    }

    return false;
}

const Protium::Allocation::Chunk* Protium::Allocation::FixedAllocator::HasBlock( void * p ) const {
    const std::size_t chunkLength = numBlocks_ * blockSize_;
    for ( ChunkCIter it( chunks_.begin() ); it != chunks_.end(); ++it )
    {
        const Chunk & chunk = *it;
        if ( chunk.HasBlock( p, chunkLength ) )
            return &chunk;
    }
    return NULL;
}


bool Protium::Allocation::FixedAllocator::TrimEmptyChunk( void )
{
    // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
    assert( ( NULL == emptyChunk_ ) || ( emptyChunk_->HasAvailable( numBlocks_ ) ) );
    if ( NULL == emptyChunk_ ) return false;

    // If emptyChunk_ points to valid Chunk, then chunk list is not empty.
    assert( !chunks_.empty() );
    // And there should be exactly 1 empty Chunk.
    assert( 1 == CountEmptyChunks() );

    Chunk * lastChunk = &chunks_.back();
    if ( lastChunk != emptyChunk_ )
        std::swap( *emptyChunk_, *lastChunk );
    assert( lastChunk->HasAvailable( numBlocks_ ) );
    lastChunk->Release();
    chunks_.pop_back();

    if ( chunks_.empty() )
    {
        allocChunk_ = NULL;
        deallocChunk_ = NULL;
    }
    else
    {
        if ( deallocChunk_ == emptyChunk_ )
        {
            deallocChunk_ = &chunks_.front();
            assert( deallocChunk_->fNAvail < numBlocks_ );
        }
        if ( allocChunk_ == emptyChunk_ )
        {
            allocChunk_ = &chunks_.back();
            assert( allocChunk_->fNAvail < numBlocks_ );
        }
    }

    emptyChunk_ = NULL;
    assert( 0 == CountEmptyChunks() );

    return true;
}

bool Protium::Allocation::FixedAllocator::TrimChunkList( void )
{
    if ( chunks_.empty() )
    {
        assert( NULL == allocChunk_ );
        assert( NULL == deallocChunk_ );
    }

    if ( chunks_.size() == chunks_.capacity() )
        return false;
    // Use the "make-a-temp-and-swap" trick to remove excess capacity.
    Chunks( chunks_ ).swap( chunks_ );

    return true;
}

// FixedAllocator::MakeNewChunk -----------------------------------------------

bool Protium::Allocation::FixedAllocator::MakeNewChunk( void )
{
    bool allocated = false;
    try
    {
        std::size_t size = chunks_.size();
        // Calling chunks_.reserve *before* creating and initializing the new
        // Chunk means that nothing is leaked by this function in case an
        // exception is thrown from reserve.
        if ( chunks_.capacity() == size )
        {
            if ( 0 == size ) size = 4;
            chunks_.reserve( size * 2 );
        }
        Chunk newChunk;
        allocated = newChunk.Init( blockSize_, numBlocks_ );
        if ( allocated )
            chunks_.push_back( newChunk );
    }
    catch ( ... )
    {
        allocated = false;
    }
    if ( !allocated ) return false;

    allocChunk_ = &chunks_.back();
    deallocChunk_ = &chunks_.front();
    return true;
}

// FixedAllocator::Allocate ---------------------------------------------------

void* Protium::Allocation::FixedAllocator::Allocate( void )
{
    // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
    assert( ( NULL == emptyChunk_ ) || ( emptyChunk_->HasAvailable( numBlocks_ ) ) );
    assert( CountEmptyChunks() < 2 );

    if ( ( NULL == allocChunk_ ) || allocChunk_->IsFilled() )
    {
        if ( NULL != emptyChunk_ )
        {
            allocChunk_ = emptyChunk_;
            emptyChunk_ = NULL;
        }
        else
        {
            for ( ChunkIter i( chunks_.begin() ); ; ++i )
            {
                if ( chunks_.end() == i )
                {
                    if ( !MakeNewChunk() )
                        return NULL;
                    break;
                }
                if ( !i->IsFilled() )
                {
                    allocChunk_ = &*i;
                    break;
                }
            }
        }
    }
    else if ( allocChunk_ == emptyChunk_)
        // detach emptyChunk_ from allocChunk_, because after 
        // calling allocChunk_->Allocate(blockSize_); the chunk 
        // is no longer empty.
        emptyChunk_ = NULL;

    assert( allocChunk_ != NULL );
    assert( !allocChunk_->IsFilled() );
    void * place = allocChunk_->Allocate( blockSize_ );

    // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
    assert( ( NULL == emptyChunk_ ) || ( emptyChunk_->HasAvailable( numBlocks_ ) ) );
    assert( CountEmptyChunks() < 2 );
#ifdef LOKI_CHECK_FOR_CORRUPTION
    if ( allocChunk_->IsCorrupt( numBlocks_, blockSize_, true ) )
    {
        assert( false );
        return NULL;
    }
#endif

    return place;
}

// FixedAllocator::Deallocate -------------------------------------------------

bool Protium::Allocation::FixedAllocator::Deallocate( void * p, Chunk * hint )
{
    assert(!chunks_.empty());
    assert(&chunks_.front() <= deallocChunk_);
    assert(&chunks_.back() >= deallocChunk_);
    assert( &chunks_.front() <= allocChunk_ );
    assert( &chunks_.back() >= allocChunk_ );
    assert( CountEmptyChunks() < 2 );

    Chunk * foundChunk = ( NULL == hint ) ? VicinityFind( p ) : hint;
    if ( NULL == foundChunk )
        return false;

    assert( foundChunk->HasBlock( p, numBlocks_ * blockSize_ ) );
#ifdef LOKI_CHECK_FOR_CORRUPTION
    if ( foundChunk->IsCorrupt( numBlocks_, blockSize_, true ) )
    {
        assert( false );
        return false;
    }
    if ( foundChunk->IsBlockAvailable( p, numBlocks_, blockSize_ ) )
    {
        assert( false );
        return false;
    }
#endif
    deallocChunk_ = foundChunk;
    DoDeallocate(p);
    assert( CountEmptyChunks() < 2 );

    return true;
}

// FixedAllocator::VicinityFind -----------------------------------------------

Protium::Allocation::Chunk* Protium::Allocation::FixedAllocator::VicinityFind( void * p ) const {
    if ( chunks_.empty() ) return NULL;
    assert(deallocChunk_);

    const std::size_t chunkLength = numBlocks_ * blockSize_;
    Chunk * lo = deallocChunk_;
    Chunk * hi = deallocChunk_ + 1;
    const Chunk * loBound = &chunks_.front();
    const Chunk * hiBound = &chunks_.back() + 1;

    // Special case: deallocChunk_ is the last in the array
    if (hi == hiBound) hi = NULL;

    for (;;)
    {
        if (lo)
        {
            if ( lo->HasBlock( p, chunkLength ) ) return lo;
            if ( lo == loBound )
            {
                lo = NULL;
                if ( NULL == hi ) break;
            }
            else --lo;
        }

        if (hi)
        {
            if ( hi->HasBlock( p, chunkLength ) ) return hi;
            if ( ++hi == hiBound )
            {
                hi = NULL;
                if ( NULL == lo ) break;
            }
        }
    }

    return NULL;
}

// FixedAllocator::DoDeallocate -----------------------------------------------

void Protium::Allocation::FixedAllocator::DoDeallocate(void* p)
{
    // Show that deallocChunk_ really owns the block at address p.
    assert( deallocChunk_->HasBlock( p, numBlocks_ * blockSize_ ) );
    // Either of the next two assertions may fail if somebody tries to
    // delete the same block twice.
    assert( emptyChunk_ != deallocChunk_ );
    assert( !deallocChunk_->HasAvailable( numBlocks_ ) );
    // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
    assert( ( NULL == emptyChunk_ ) || ( emptyChunk_->HasAvailable( numBlocks_ ) ) );

    // call into the chunk, will adjust the inner list but won't release memory
    deallocChunk_->Deallocate(p, blockSize_);

    if ( deallocChunk_->HasAvailable( numBlocks_ ) )
    {
        assert( emptyChunk_ != deallocChunk_ );
        // deallocChunk_ is empty, but a Chunk is only released if there are 2
        // empty chunks.  Since emptyChunk_ may only point to a previously
        // cleared Chunk, if it points to something else besides deallocChunk_,
        // then FixedAllocator currently has 2 empty Chunks.
        if ( NULL != emptyChunk_ )
        {
            // If last Chunk is empty, just change what deallocChunk_
            // points to, and release the last.  Otherwise, swap an empty
            // Chunk with the last, and then release it.
            Chunk * lastChunk = &chunks_.back();
            if ( lastChunk == deallocChunk_ )
                deallocChunk_ = emptyChunk_;
            else if ( lastChunk != emptyChunk_ )
                std::swap( *emptyChunk_, *lastChunk );
            assert( lastChunk->HasAvailable( numBlocks_ ) );
            lastChunk->Release();
            chunks_.pop_back();
            if ( ( allocChunk_ == lastChunk ) || allocChunk_->IsFilled() ) 
                allocChunk_ = deallocChunk_;
        }
        emptyChunk_ = deallocChunk_;
    }

    // prove either emptyChunk_ points nowhere, or points to a truly empty Chunk.
    assert( ( NULL == emptyChunk_ ) || ( emptyChunk_->HasAvailable( numBlocks_ ) ) );
}
