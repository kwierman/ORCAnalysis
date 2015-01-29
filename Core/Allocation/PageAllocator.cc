#include "ORCA/Allocation/PageAllocator.hh"

#include <assert.h>

unsigned char ORCA::Allocation::PageAllocator::MinObjectsPerPage_ = 8;
unsigned char ORCA::Allocation::PageAllocator::MaxObjectsPerPage_ = 255;

ORCA::Allocation::PageAllocator::PageAllocator()
    : blockSize_( 0 )
    , numBlocks_( 0 )
    , Pages_( 0 )
    , allocPage_( NULL )
    , deallocPage_( NULL )
    , emptyPage_( NULL )
{
}

ORCA::Allocation::PageAllocator::~PageAllocator()
{
    for ( PageIter i( Pages_.begin() ); i != Pages_.end(); ++i )
       i->Release();
}


void ORCA::Allocation::PageAllocator::Initialize( std::size_t blockSize, std::size_t pageSize )
{
    assert( blockSize > 0 );
    assert( pageSize >= blockSize );
    blockSize_ = blockSize;

    std::size_t numBlocks = pageSize / blockSize;
    if ( numBlocks > MaxObjectsPerPage_ ) numBlocks = MaxObjectsPerPage_;
    else if ( numBlocks < MinObjectsPerPage_ ) numBlocks = MinObjectsPerPage_;

    numBlocks_ = static_cast<unsigned char>(numBlocks);
    assert(numBlocks_ == numBlocks);
}

std::size_t ORCA::Allocation::PageAllocator::CountEmptyPages( void ) const{
    return ( NULL == emptyPage_ ) ? 0 : 1;
}

bool ORCA::Allocation::PageAllocator::IsCorrupt( void ) const {
    const bool isEmpty = Pages_.empty();
    PageCIter start( Pages_.begin() );
    PageCIter last( Pages_.end() );
    const size_t emptyPageCount = CountEmptyPages();

    if ( isEmpty )
    {
        if ( start != last )
        {
            assert( false );
            return true;
        }
        if ( 0 < emptyPageCount )
        {
            assert( false );
            return true;
        }
        if ( NULL != deallocPage_ )
        {
            assert( false );
            return true;
        }
        if ( NULL != allocPage_ )
        {
            assert( false );
            return true;
        }
        if ( NULL != emptyPage_ )
        {
            assert( false );
            return true;
        }
    }

    else
    {
        const Page * front = &Pages_.front();
        const Page * back  = &Pages_.back();
        if ( start >= last )
        {
            assert( false );
            return true;
        }
        if ( back < deallocPage_ )
        {
            assert( false );
            return true;
        }
        if ( back < allocPage_ )
        {
            assert( false );
            return true;
        }
        if ( front > deallocPage_ )
        {
            assert( false );
            return true;
        }
        if ( front > allocPage_ )
        {
            assert( false );
            return true;
        }

        switch ( emptyPageCount )
        {
            case 0:
                if ( emptyPage_ != NULL )
                {
                    assert( false );
                    return true;
                }
                break;
            case 1:
                if ( emptyPage_ == NULL )
                {
                    assert( false );
                    return true;
                }
                if ( back < emptyPage_ )
                {
                    assert( false );
                    return true;
                }
                if ( front > emptyPage_ )
                {
                    assert( false );
                    return true;
                }
                if ( !emptyPage_->HasAvailable( numBlocks_ ) )
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
        for ( PageCIter it( start ); it != last; ++it )
        {
            const Page & Page = *it;
            if ( Page.IsCorrupt( numBlocks_, blockSize_, true ) )
                return true;
        }
    }

    return false;
}

const ORCA::Allocation::Page* ORCA::Allocation::PageAllocator::HasBlock( void * p ) const {
    const std::size_t PageLength = numBlocks_ * blockSize_;
    for ( PageCIter it( Pages_.begin() ); it != Pages_.end(); ++it )
    {
        const Page & Page = *it;
        if ( Page.HasBlock( p, PageLength ) )
            return &Page;
    }
    return NULL;
}


bool ORCA::Allocation::PageAllocator::TrimEmptyPage( void )
{
    // prove either emptyPage_ points nowhere, or points to a truly empty Page.
    assert( ( NULL == emptyPage_ ) || ( emptyPage_->HasAvailable( numBlocks_ ) ) );
    if ( NULL == emptyPage_ ) return false;

    // If emptyPage_ points to valid Page, then Page list is not empty.
    assert( !Pages_.empty() );
    // And there should be exactly 1 empty Page.
    assert( 1 == CountEmptyPages() );

    Page * lastPage = &Pages_.back();
    if ( lastPage != emptyPage_ )
        std::swap( *emptyPage_, *lastPage );
    assert( lastPage->HasAvailable( numBlocks_ ) );
    lastPage->Release();
    Pages_.pop_back();

    if ( Pages_.empty() )
    {
        allocPage_ = NULL;
        deallocPage_ = NULL;
    }
    else
    {
        if ( deallocPage_ == emptyPage_ )
        {
            deallocPage_ = &Pages_.front();
            assert( deallocPage_->fNAvail < numBlocks_ );
        }
        if ( allocPage_ == emptyPage_ )
        {
            allocPage_ = &Pages_.back();
            assert( allocPage_->fNAvail < numBlocks_ );
        }
    }

    emptyPage_ = NULL;
    assert( 0 == CountEmptyPages() );

    return true;
}

bool ORCA::Allocation::PageAllocator::TrimPageList( void )
{
    if ( Pages_.empty() )
    {
        assert( NULL == allocPage_ );
        assert( NULL == deallocPage_ );
    }

    if ( Pages_.size() == Pages_.capacity() )
        return false;
    // Use the "make-a-temp-and-swap" trick to remove excess capacity.
    Pages( Pages_ ).swap( Pages_ );

    return true;
}

// PageAllocator::MakeNewPage -----------------------------------------------

bool ORCA::Allocation::PageAllocator::MakeNewPage( void )
{
    bool allocated = false;
    try
    {
        std::size_t size = Pages_.size();
        // Calling Pages_.reserve *before* creating and initializing the new
        // Page means that nothing is leaked by this function in case an
        // exception is thrown from reserve.
        if ( Pages_.capacity() == size )
        {
            if ( 0 == size ) size = 4;
            Pages_.reserve( size * 2 );
        }
        Page newPage;
        allocated = newPage.Init( blockSize_, numBlocks_ );
        if ( allocated )
            Pages_.push_back( newPage );
    }
    catch ( ... )
    {
        allocated = false;
    }
    if ( !allocated ) return false;

    allocPage_ = &Pages_.back();
    deallocPage_ = &Pages_.front();
    return true;
}

// PageAllocator::Allocate ---------------------------------------------------

void* ORCA::Allocation::PageAllocator::Allocate( void )
{
    // prove either emptyPage_ points nowhere, or points to a truly empty Page.
    assert( ( NULL == emptyPage_ ) || ( emptyPage_->HasAvailable( numBlocks_ ) ) );
    assert( CountEmptyPages() < 2 );

    if ( ( NULL == allocPage_ ) || allocPage_->IsFilled() )
    {
        if ( NULL != emptyPage_ )
        {
            allocPage_ = emptyPage_;
            emptyPage_ = NULL;
        }
        else
        {
            for ( PageIter i( Pages_.begin() ); ; ++i )
            {
                if ( Pages_.end() == i )
                {
                    if ( !MakeNewPage() )
                        return NULL;
                    break;
                }
                if ( !i->IsFilled() )
                {
                    allocPage_ = &*i;
                    break;
                }
            }
        }
    }
    else if ( allocPage_ == emptyPage_)
        // detach emptyPage_ from allocPage_, because after 
        // calling allocPage_->Allocate(blockSize_); the Page 
        // is no longer empty.
        emptyPage_ = NULL;

    assert( allocPage_ != NULL );
    assert( !allocPage_->IsFilled() );
    void * place = allocPage_->Allocate( blockSize_ );

    // prove either emptyPage_ points nowhere, or points to a truly empty Page.
    assert( ( NULL == emptyPage_ ) || ( emptyPage_->HasAvailable( numBlocks_ ) ) );
    assert( CountEmptyPages() < 2 );
#ifdef LOKI_CHECK_FOR_CORRUPTION
    if ( allocPage_->IsCorrupt( numBlocks_, blockSize_, true ) )
    {
        assert( false );
        return NULL;
    }
#endif

    return place;
}

// PageAllocator::Deallocate -------------------------------------------------

bool ORCA::Allocation::PageAllocator::Deallocate( void * p, Page * hint )
{
    assert(!Pages_.empty());
    assert(&Pages_.front() <= deallocPage_);
    assert(&Pages_.back() >= deallocPage_);
    assert( &Pages_.front() <= allocPage_ );
    assert( &Pages_.back() >= allocPage_ );
    assert( CountEmptyPages() < 2 );

    Page * foundPage = ( NULL == hint ) ? VicinityFind( p ) : hint;
    if ( NULL == foundPage )
        return false;

    assert( foundPage->HasBlock( p, numBlocks_ * blockSize_ ) );
#ifdef LOKI_CHECK_FOR_CORRUPTION
    if ( foundPage->IsCorrupt( numBlocks_, blockSize_, true ) )
    {
        assert( false );
        return false;
    }
    if ( foundPage->IsBlockAvailable( p, numBlocks_, blockSize_ ) )
    {
        assert( false );
        return false;
    }
#endif
    deallocPage_ = foundPage;
    DoDeallocate(p);
    assert( CountEmptyPages() < 2 );

    return true;
}

// PageAllocator::VicinityFind -----------------------------------------------

ORCA::Allocation::Page* ORCA::Allocation::PageAllocator::VicinityFind( void * p ) const {
    if ( Pages_.empty() ) return NULL;
    assert(deallocPage_);

    const std::size_t PageLength = numBlocks_ * blockSize_;
    Page * lo = deallocPage_;
    Page * hi = deallocPage_ + 1;
    const Page * loBound = &Pages_.front();
    const Page * hiBound = &Pages_.back() + 1;

    // Special case: deallocPage_ is the last in the array
    if (hi == hiBound) hi = NULL;

    for (;;)
    {
        if (lo)
        {
            if ( lo->HasBlock( p, PageLength ) ) return lo;
            if ( lo == loBound )
            {
                lo = NULL;
                if ( NULL == hi ) break;
            }
            else --lo;
        }

        if (hi)
        {
            if ( hi->HasBlock( p, PageLength ) ) return hi;
            if ( ++hi == hiBound )
            {
                hi = NULL;
                if ( NULL == lo ) break;
            }
        }
    }

    return NULL;
}

// PageAllocator::DoDeallocate -----------------------------------------------

void ORCA::Allocation::PageAllocator::DoDeallocate(void* p)
{
    // Show that deallocPage_ really owns the block at address p.
    assert( deallocPage_->HasBlock( p, numBlocks_ * blockSize_ ) );
    // Either of the next two assertions may fail if somebody tries to
    // delete the same block twice.
    assert( emptyPage_ != deallocPage_ );
    assert( !deallocPage_->HasAvailable( numBlocks_ ) );
    // prove either emptyPage_ points nowhere, or points to a truly empty Page.
    assert( ( NULL == emptyPage_ ) || ( emptyPage_->HasAvailable( numBlocks_ ) ) );

    // call into the Page, will adjust the inner list but won't release memory
    deallocPage_->Deallocate(p, blockSize_);

    if ( deallocPage_->HasAvailable( numBlocks_ ) )
    {
        assert( emptyPage_ != deallocPage_ );
        // deallocPage_ is empty, but a Page is only released if there are 2
        // empty Pages.  Since emptyPage_ may only point to a previously
        // cleared Page, if it points to something else besides deallocPage_,
        // then PageAllocator currently has 2 empty Pages.
        if ( NULL != emptyPage_ )
        {
            // If last Page is empty, just change what deallocPage_
            // points to, and release the last.  Otherwise, swap an empty
            // Page with the last, and then release it.
            Page * lastPage = &Pages_.back();
            if ( lastPage == deallocPage_ )
                deallocPage_ = emptyPage_;
            else if ( lastPage != emptyPage_ )
                std::swap( *emptyPage_, *lastPage );
            assert( lastPage->HasAvailable( numBlocks_ ) );
            lastPage->Release();
            Pages_.pop_back();
            if ( ( allocPage_ == lastPage ) || allocPage_->IsFilled() ) 
                allocPage_ = deallocPage_;
        }
        emptyPage_ = deallocPage_;
    }

    // prove either emptyPage_ points nowhere, or points to a truly empty Page.
    assert( ( NULL == emptyPage_ ) || ( emptyPage_->HasAvailable( numBlocks_ ) ) );
}
