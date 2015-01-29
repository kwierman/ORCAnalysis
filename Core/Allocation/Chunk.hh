/*! \file Chunk.h
	\author kwierman
 */

#ifndef Protium_Chunk_hh_
#define Protium_Chunk_hh_

#include <cstddef>

namespace Protium{

	namespace Allocation{

		//! Represents a fixed number of blocks 

		class Chunk{

			/**Data **/
        	unsigned char* fData;

        	//! First Available block
	        unsigned char fFirstAvailable;

	        //! Number of available blocks
	        unsigned char fNAvail;

	        //! Since all members are private, only FixedAllocator can access them.
			friend class FixedAllocator;
			//! Each time a new chunk is created, it must be initialized (or when reused in a pool)
			//! \param size the size per chunk
			//! \param n the number of blocks
        	bool Init( std::size_t size, unsigned char n );
        	//! Each time a chunk is reused, it needs to be reset
	        void Reset( std::size_t blockSize, unsigned char blocks );
	        //! Deallocates all blocks in chunck
	        void Release();
	        //! Allocates out a number of additional blocks
        	void* Allocate( std::size_t blocks );
        	//! DeAllocates a number of blocks starting at pointer
	        void Deallocate( void * p, std::size_t blocks );
	        /*! Determines whether this chunk is corrupt
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
	        //! Checks if this block belongs to this chunk
	        inline bool HasBlock( void* p, std::size_t chunkLength ) const {
	            unsigned char * pc = static_cast< unsigned char * >( p );
	            return ( fData <= pc ) && ( pc < fData + chunkLength );
	        }
	        //! Checks to see if this number of blocks is available in this chunk
	        inline bool HasAvailable( unsigned char numBlocks ) const {
	        	return ( fNAvail == numBlocks ); 
	        }
	        //! Checks to see if this is filled
	        inline bool IsFilled( void ) const { 
	        	return ( 0 == fNAvail ); 
	        }
		};//end Chunk class
	}//End Allocation Namespace
}//End Protium Namespace



#endif //File Guardian