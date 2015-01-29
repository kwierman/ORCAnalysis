/*! \file Page.h
	\author kwierman
 */

#ifndef ORCA_Page_hh_
#define ORCA_Page_hh_

#include <cstddef>

namespace ORCA{

	namespace Allocation{

		//! Represents a fixed number of blocks 

		class Page{

			/**Data **/
        	unsigned char* fData;

        	//! First Available block
	        unsigned char fFirstAvailable;

	        //! Number of available blocks
	        unsigned char fNAvail;

	        //! Since all members are private, only FixedAllocator can access them.
			friend class FixedAllocator;
			//! Each time a new Page is created, it must be initialized (or when reused in a pool)
			//! \param size the size per Page
			//! \param n the number of blocks
        	bool Init( std::size_t size, unsigned char n );
        	//! Each time a Page is reused, it needs to be reset
	        void Reset( std::size_t blockSize, unsigned char blocks );
	        //! Deallocates all blocks in chunck
	        void Release();
	        //! Allocates out a number of additional blocks
        	void* Allocate( std::size_t blocks );
        	//! DeAllocates a number of blocks starting at pointer
	        void Deallocate( void * p, std::size_t blocks );
	        /*! Determines whether this Page is corrupt
	        	The following conditions imply corruption:

	        	- Number of available blocks exceeds the number of blocks contained
	        	- The last block occurs before the first block (overwrite)
	        	- For each block:
	        	    - If block is found outside outside of bounds
	        	    - Block occurs twice
	        */
	        bool IsCorrupt( unsigned char numBlocks, std::size_t blockSize, bool checkIndexes ) const;
	        //! Checks the block starting at p and for p+numBlocks*blockSize checks to see if they're used
	        bool IsBlockAvailable( void* p, unsigned char numBlocks, std::size_t blockSize ) const;
	        //! Checks if this block belongs to this Page
	        inline bool HasBlock( void* p, std::size_t PageLength ) const {
	            unsigned char * pc = static_cast< unsigned char * >( p );
	            return ( fData <= pc ) && ( pc < fData + PageLength );
	        }
	        //! Checks to see if this number of blocks is available in this Page
	        inline bool HasAvailable( unsigned char numBlocks ) const {
	        	return ( fNAvail == numBlocks ); 
	        }
	        //! Checks to see if this is filled
	        inline bool IsFilled( void ) const { 
	        	return ( 0 == fNAvail ); 
	        }
		};//end Page class
	}//End Allocation Namespace
}//End ORCA Namespace



#endif //File Guardian