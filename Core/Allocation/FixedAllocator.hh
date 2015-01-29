#ifndef Protium_FixedAllocator_hh_
#define Protium_FixedAllocator_hh_

#include "Protium/Allocation/Chunk.hh"

#include <vector>

namespace Protium{

	namespace Allocation{
	
        //! Pool of allocation chunks
	    class FixedAllocator{

    	private:

    		//! Deallocated the chunk located at p
    		//! \param p points to the chunk
    		//! \warning Requires the internal pointer to be at the chunk in question
	        void DoDeallocate( void * p );

            /*! Creates an empty Chunk and adds it to the end of the ChunkList.
             All calls to the lower-level memory allocation functions occur inside
             this function, and so the only try-catch block is inside here.
             @return true for success, false for failure.
             */
            bool MakeNewChunk( void );

            /*! Finds the Chunk which owns the block at address p.  It starts at
             deallocChunk_ and searches in both forwards and backwards directions
             from there until it finds the Chunk which owns p.  This algorithm
             should find the Chunk quickly if it is deallocChunk_ or is close to it
             in the Chunks container.  This goes both forwards and backwards since
             that works well for both same-order and opposite-order deallocations.
             (Same-order = objects are deallocated in the same order in which they
             were allocated.  Opposite order = objects are deallocated in a last to
             first order.  Complexity is O(C) where C is count of all Chunks.  This
             never throws.
             @return Pointer to Chunk that owns p, or NULL if no owner found.
             */
            Chunk * VicinityFind( void * p ) const;

            //! Not implemented.
            FixedAllocator(const FixedAllocator&);
            //! Not implemented.
            FixedAllocator& operator=(const FixedAllocator&);

            //! Type of container used to hold Chunks.
            typedef std::vector< Chunk > Chunks;
            //! Iterator through container of Chunks.
            typedef Chunks::iterator ChunkIter;
            //! Iterator through const container of Chunks.
            typedef Chunks::const_iterator ChunkCIter;

            /// Fewest # of objects managed by a Chunk.
            static unsigned char MinObjectsPerChunk_;

            /// Most # of objects managed by a Chunk - never exceeds UCHAR_MAX.
            static unsigned char MaxObjectsPerChunk_;

            /// Number of bytes in a single block within a Chunk.
            std::size_t blockSize_;
            /// Number of blocks managed by each Chunk.
            unsigned char numBlocks_;

            /// Container of Chunks.
            Chunks chunks_;
            /// Pointer to Chunk used for last or next allocation.
            Chunk * allocChunk_;
            /// Pointer to Chunk used for last or next deallocation.
            Chunk * deallocChunk_;
            /// Pointer to the only empty Chunk if there is one, else NULL.
            Chunk * emptyChunk_;

        public:
            /// Create a FixedAllocator which manages blocks of 'blockSize' size.
            FixedAllocator();

            /// Destroy the FixedAllocator and release all its Chunks.
            ~FixedAllocator();

            /// Initializes a FixedAllocator by calculating # of blocks per Chunk.
            void Initialize( std::size_t blockSize, std::size_t pageSize );

            /** Returns pointer to allocated memory block of fixed size - or NULL
             if it failed to allocate.
             */
            void * Allocate( void );

            /** Deallocate a memory block previously allocated with Allocate.  If
             the block is not owned by this FixedAllocator, it returns false so
             that SmallObjAllocator can call the default deallocator.  If the
             block was found, this returns true.
             */
            bool Deallocate( void * p, Chunk * hint );

            /// Returns block size with which the FixedAllocator was initialized.
            inline std::size_t BlockSize() const { return blockSize_; }

            /** Releases the memory used by the empty Chunk.  This will take
             constant time under any situation.
             @return True if empty chunk found and released, false if none empty.
             */
            bool TrimEmptyChunk( void );

            /** Releases unused spots from ChunkList.  This takes constant time
             with respect to # of Chunks, but actual time depends on underlying
             memory allocator.
             @return False if no unused spots, true if some found and released.
             */
            bool TrimChunkList( void );

            /** Returns count of empty Chunks held by this allocator.  Complexity
             is O(C) where C is the total number of Chunks - empty or used.
             */
            std::size_t CountEmptyChunks( void ) const;

            /** Determines if FixedAllocator is corrupt.  Checks data members to
             see if any have erroneous values, or violate class invariants.  It
             also checks if any Chunk is corrupt.  Complexity is O(C) where C is
             the number of Chunks.  If any data is corrupt, this will return true
             in release mode, or assert in debug mode.
             */
            bool IsCorrupt( void ) const;

            /** Returns true if the block at address p is within a Chunk owned by
             this FixedAllocator.  Complexity is O(C) where C is the total number
             of Chunks - empty or used.
             */
            const Chunk * HasBlock( void * p ) const;
            inline Chunk * HasBlock( void * p )
            {
                return const_cast< Chunk * >(
                    const_cast< const FixedAllocator * >( this )->HasBlock( p ) );
            }

        };//Class FixedAllocator

	}//Namespace Allocation

}//Namespace Protium

#endif //File Guardian