#include "Protium/Allocation/Chunk.hh"

#include <assert.h>
#include <bitset>

bool Protium::Allocation::Chunk::Init( std::size_t blockSize, unsigned char blocks ){
    assert(blockSize > 0 && blocks > 0 );
    const std::size_t totalSize= blockSize*blocks;
    assert( totalSize / blockSize == blocks);
    fData = static_cast<unsigned char* > (operator new(totalSize) );
    Reset( blockSize, blocks );
    return true;
}

void Protium::Allocation::Chunk::Reset(std::size_t blockSize, unsigned char blocks){
    assert(blockSize > 0 && blocks > 0);
    fFirstAvailable = 0;
    fNAvail = blocks;
    unsigned char i = 0;
    for ( unsigned char * p = fData; i != blocks; p += blockSize )
        *p = ++i;
}

void Protium::Allocation::Chunk::Release(){
    assert( fData != NULL );
    ::operator delete ( fData );
}


void* Protium::Allocation::Chunk::Allocate(std::size_t blockSize){
    if ( IsFilled() ) return NULL;

    assert((fFirstAvailable * blockSize) / blockSize == fFirstAvailable);

    unsigned char* result = fData + (fFirstAvailable * blockSize);
    fFirstAvailable = *result;
    --fNAvail;

    return result;
}


void Protium::Allocation::Chunk::Deallocate(void* p, std::size_t blockSize){
    assert(p >= fData);

    unsigned char* toRelease = static_cast<unsigned char*>(p);
    // Alignment check
    assert((toRelease - fData) % blockSize == 0);
    unsigned char index = static_cast< unsigned char >(
        ( toRelease - fData ) / blockSize);


    *toRelease = fFirstAvailable;
    fFirstAvailable = index;
    // Truncation check
    assert(fFirstAvailable == (toRelease - fData) / blockSize);

    ++fNAvail;
}

bool Protium::Allocation::Chunk::IsCorrupt( unsigned char numBlocks, std::size_t blockSize, bool checkIndexes ) const{

    if ( numBlocks < fNAvail )
    {
        // Contents at this Chunk corrupted.  This might mean something has
        // overwritten memory owned by the Chunks container.
        assert( false );
        return true;
    }
    if ( IsFilled() )
        // Useless to do further corruption checks if all blocks allocated.
        return false;
    unsigned char index = fFirstAvailable;
    if ( numBlocks <= index )
    {
        // Contents at this Chunk corrupted.  This might mean something has
        // overwritten memory owned by the Chunks container.
        assert( false );
        return true;
    }
    if ( !checkIndexes )
        // Caller chose to skip more complex corruption tests.
        return false;

    /* If the bit at index was set in foundBlocks, then the stealth index was
     found on the linked-list.
     */
    std::bitset< 256 > foundBlocks;
    unsigned char * nextBlock = NULL;

    /* The loop goes along singly linked-list of stealth indexes and makes sure
     that each index is within bounds (0 <= index < numBlocks) and that the
     index was not already found while traversing the linked-list.  The linked-
     list should have exactly fNAvail nodes, so the for loop will not
     check more than fNAvail.  This loop can't check inside allocated
     blocks for corruption since such blocks are not within the linked-list.
     Contents of allocated blocks are not changed by Chunk.

     Here are the types of corrupted link-lists which can be verified.  The
     corrupt index is shown with asterisks in each example.

     Type 1: Index is too big.
      numBlocks == 64
      fNAvail == 7
      fFirstAvailable -> 17 -> 29 -> *101*
      There should be no indexes which are equal to or larger than the total
      number of blocks.  Such an index would refer to a block beyond the
      Chunk's allocated domain.

     Type 2: Index is repeated.
      numBlocks == 64
      fNAvail == 5
      fFirstAvailable -> 17 -> 29 -> 53 -> *17* -> 29 -> 53 ...
      No index should be repeated within the linked-list since that would
      indicate the presence of a loop in the linked-list.
     */
    for ( unsigned char cc = 0; ; )
    {
        nextBlock = fData + ( index * blockSize );
        foundBlocks.set( index, true );
        ++cc;
        if ( cc >= fNAvail )
            // Successfully counted off number of nodes in linked-list.
            break;
        index = *nextBlock;
        if ( numBlocks <= index )
        {
            /* This catches Type 1 corruptions as shown in above comments.
             This implies that a block was corrupted due to a stray pointer
             or an operation on a nearby block overran the size of the block.
             */
            assert( false );
            return true;
        }
        if ( foundBlocks.test( index ) )
        {
            /* This catches Type 2 corruptions as shown in above comments.
             This implies that a block was corrupted due to a stray pointer
             or an operation on a nearby block overran the size of the block.
             Or perhaps the program tried to delete a block more than once.
             */
            assert( false );
            return true;
        }
    }
    if ( foundBlocks.count() != fNAvail )
    {
        /* This implies that the singly-linked-list of stealth indexes was
         corrupted.  Ideally, this should have been detected within the loop.
         */
        assert( false );
        return true;
    }

    return false;
}

// Chunk::IsBlockAvailable ----------------------------------------------------

bool Protium::Allocation::Chunk::IsBlockAvailable( void * p, unsigned char numBlocks,
    std::size_t blockSize ) const
{
    (void) numBlocks;
    
    if ( IsFilled() )
        return false;

    unsigned char * place = static_cast< unsigned char * >( p );
    // Alignment check
    assert( ( place - fData ) % blockSize == 0 );
    unsigned char blockIndex = static_cast< unsigned char >(
        ( place - fData ) / blockSize );

    unsigned char index = fFirstAvailable;
    assert( numBlocks > index );
    if ( index == blockIndex )
        return true;

    /* If the bit at index was set in foundBlocks, then the stealth index was
     found on the linked-list.
     */
    std::bitset< 256 > foundBlocks;
    unsigned char * nextBlock = NULL;
    for ( unsigned char cc = 0; ; )
    {
        nextBlock = fData + ( index * blockSize );
        foundBlocks.set( index, true );
        ++cc;
        if ( cc >= fNAvail )
            // Successfully counted off number of nodes in linked-list.
            break;
        index = *nextBlock;
        if ( index == blockIndex )
            return true;
        assert( numBlocks > index );
        assert( !foundBlocks.test( index ) );
    }

    return false;
}