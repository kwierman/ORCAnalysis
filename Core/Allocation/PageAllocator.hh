#ifndef ORCA_FixedAllocator_hh_
#define ORCA_FixedAllocator_hh_

#include "ORCA/Allocation/Page.hh"

#include <vector>

namespace ORCA{

      namespace Allocation{
      
        //! Pool of allocation Pages
          class FixedAllocator{

          private:

            //! Deallocated the Page located at p
            //! \param p points to the Page
            //! \warning Requires the internal pointer to be at the Page in question
            void DoDeallocate( void * p );

            /*! Creates an empty Page and adds it to the end of the PageList.
             All calls to the lower-level memory allocation functions occur inside
             this function, and so the only try-catch block is inside here.
             @return true for success, false for failure.
             */
            bool MakeNewPage( void );

            /*! Finds the Page which owns the block at address p.  It starts at
             deallocPage_ and searches in both forwards and backwards directions
             from there until it finds the Page which owns p.  This algorithm
             should find the Page quickly if it is deallocPage_ or is close to it
             in the Pages container.  This goes both forwards and backwards since
             that works well for both same-order and opposite-order deallocations.
             (Same-order = objects are deallocated in the same order in which they
             were allocated.  Opposite order = objects are deallocated in a last to
             first order.  Complexity is O(C) where C is count of all Pages.  This
             never throws.
             @return Pointer to Page that owns p, or NULL if no owner found.
             */
            Page * VicinityFind( void * p ) const;

            //! Not implemented.
            FixedAllocator(const FixedAllocator&);
            //! Not implemented.
            FixedAllocator& operator=(const FixedAllocator&);

            //! Type of container used to hold Pages.
            typedef std::vector< Page > Pages;
            //! Iterator through container of Pages.
            typedef Pages::iterator PageIter;
            //! Iterator through const container of Pages.
            typedef Pages::const_iterator PageCIter;

            /// Fewest # of objects managed by a Page.
            static unsigned char MinObjectsPerPage_;

            /// Most # of objects managed by a Page - never exceeds UCHAR_MAX.
            static unsigned char MaxObjectsPerPage_;

            /// Number of bytes in a single block within a Page.
            std::size_t blockSize_;
            /// Number of blocks managed by each Page.
            unsigned char numBlocks_;

            /// Container of Pages.
            Pages Pages_;
            /// Pointer to Page used for last or next allocation.
            Page * allocPage_;
            /// Pointer to Page used for last or next deallocation.
            Page * deallocPage_;
            /// Pointer to the only empty Page if there is one, else NULL.
            Page * emptyPage_;

        public:
            /// Create a FixedAllocator which manages blocks of 'blockSize' size.
            FixedAllocator();

            /// Destroy the FixedAllocator and release all its Pages.
            ~FixedAllocator();

            /// Initializes a FixedAllocator by calculating # of blocks per Page.
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
            bool Deallocate( void * p, Page * hint );

            /// Returns block size with which the FixedAllocator was initialized.
            inline std::size_t BlockSize() const { return blockSize_; }

            /** Releases the memory used by the empty Page.  This will take
             constant time under any situation.
             @return True if empty Page found and released, false if none empty.
             */
            bool TrimEmptyPage( void );

            /** Releases unused spots from PageList.  This takes constant time
             with respect to # of Pages, but actual time depends on underlying
             memory allocator.
             @return False if no unused spots, true if some found and released.
             */
            bool TrimPageList( void );

            /** Returns count of empty Pages held by this allocator.  Complexity
             is O(C) where C is the total number of Pages - empty or used.
             */
            std::size_t CountEmptyPages( void ) const;

            /** Determines if FixedAllocator is corrupt.  Checks data members to
             see if any have erroneous values, or violate class invariants.  It
             also checks if any Page is corrupt.  Complexity is O(C) where C is
             the number of Pages.  If any data is corrupt, this will return true
             in release mode, or assert in debug mode.
             */
            bool IsCorrupt( void ) const;

            /** Returns true if the block at address p is within a Page owned by
             this FixedAllocator.  Complexity is O(C) where C is the total number
             of Pages - empty or used.
             */
            const Page * HasBlock( void * p ) const;
            inline Page * HasBlock( void * p )
            {
                return const_cast< Page * >(
                    const_cast< const FixedAllocator * >( this )->HasBlock( p ) );
            }

        };//Class FixedAllocator

      }//Namespace Allocation

}//Namespace ORCA

#endif //File Guardian