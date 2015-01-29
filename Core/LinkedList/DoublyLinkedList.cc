#include "ORCA/LinkedList/DoublyLinkedList.hh"
#include <algorithm>
#include <cassert>

ORCA::LinkedList::DoubleLinkedNode::DoubleLinkedNode(const ORCA::LinkedList::DoubleLinkedNode& rhs) {
    fPrev = &rhs;
    fNext = rhs.fNext;
    fPrev->fNext = this;
    fNext->fPrev = this;
}

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

unsigned int ORCA::LinkedList::DoubleLinkedNode::CountPrevCycle( const ORCA::LinkedList::DoubleLinkedNode * pThis )
{
    if ( NULL == pThis )
        return 0;
    const ORCA::LinkedList::DoubleLinkedNode * p = pThis->fPrev;
    if ( NULL == p )
        return 0;
    if ( pThis == p )
        return 1;

    unsigned int count = 1;
    do
    {
        p = p->fPrev;
        ++count;
    } while ( p != pThis );

    return count;
}

// ----------------------------------------------------------------------------

unsigned int ORCA::LinkedList::DoubleLinkedNode::CountNextCycle( const ORCA::LinkedList::DoubleLinkedNode * pThis )
{
    if ( NULL == pThis )
        return 0;
    const ORCA::LinkedList::DoubleLinkedNode * p = pThis->fNext;
    if ( NULL == p )
        return 0;
    if ( pThis == p )
        return 1;

    unsigned int count = 1;
    while ( p != pThis )
    {
        p = p->fNext;
        ++count;
    }

    return count;
}

// ----------------------------------------------------------------------------

bool ORCA::LinkedList::DoubleLinkedNode::HasPrevNode( const ORCA::LinkedList::DoubleLinkedNode * p ) const
{
    if ( this == p )
        return true;
    const ORCA::LinkedList::DoubleLinkedNode * prev = fPrev;
    if ( NULL == prev )
        return false;
    while ( prev != this )
    {
        if ( p == prev )
            return true;
        prev = prev->fPrev;
    }
    return false;
}

// ----------------------------------------------------------------------------

bool ORCA::LinkedList::DoubleLinkedNode::HasNextNode( const ORCA::LinkedList::DoubleLinkedNode * p ) const
{
    if ( this == p )
        return true;
    const ORCA::LinkedList::DoubleLinkedNode * next = fNext;
    if ( NULL == next )
        return false;
    while ( next != this )
    {
        if ( p == next )
            return true;
        next = next->fNext;
    }
    return false;
}

// ----------------------------------------------------------------------------

bool ORCA::LinkedList::DoubleLinkedNode::MergeWith( ORCA::LinkedList::DoubleLinkedNode & rhs )
{

    if ( NULL == fNext )
    {
        assert( NULL == fPrev );
        return false;
    }
    ORCA::LinkedList::DoubleLinkedNode * prhs = &rhs;
    if ( prhs == this )
        return true;
    if ( NULL == prhs->fNext )
    {
        assert( NULL == prhs->fPrev );
        return true;
    }

    assert( CountPrevCycle( this ) == CountNextCycle( this ) );
    assert( CountPrevCycle( prhs ) == CountNextCycle( prhs ) );
    // If rhs node is already in this cycle, then no need to merge.
    if ( HasPrevNode( &rhs ) )
    {
        assert( HasNextNode( &rhs ) );
        return true;
    }

    if ( prhs == prhs->fNext )
    {
        /// rhs is in a cycle with 1 node.
        assert( prhs->fPrev == prhs );
        prhs->fPrev = fPrev;
        prhs->fNext = this;
        fPrev->fNext = prhs;
        fPrev = prhs;
    }
    else if ( this == fNext )
    {
        /// this is in a cycle with 1 node.
        assert( fPrev == this );
        fPrev = prhs->fPrev;
        fNext = prhs;
        prhs->fPrev->fNext = this;
        prhs->fPrev = this;
    }
    else
    {
        fNext->fPrev = prhs->fPrev;
        prhs->fPrev->fNext = fPrev;
        fNext = prhs;
        prhs->fPrev = this;
    }

    assert( CountPrevCycle( this ) == CountNextCycle( this ) );
    return true;
}

bool ORCA::LinkedList::DoubleLinkedNode::RemoveThis()
{


    if ( NULL == fNext )
    {
        assert( NULL == fPrev );
        // Return false so it does not try to destroy shared object
        // more than once.
        return false;
    }
    else if (fNext == this)
    {   
        assert(fPrev == this);
        // Set these to NULL to prevent re-entrancy.
        fPrev = NULL;
        fNext = NULL;
        return true;
    }



    fPrev->fNext = fNext;
    fNext->fPrev = fPrev;

    return false;
}

// ----------------------------------------------------------------------------

void ORCA::LinkedList::DoubleLinkedNode::SwapWith(ORCA::LinkedList::DoubleLinkedNode& rhs)
{

    if (fNext == this)
    {
        assert(fPrev == this);
        if (rhs.fNext == &rhs)
        {
            assert(rhs.fPrev == &rhs);
            // both lists are empty, nothing 2 do
            return;
        }
        fPrev = rhs.fPrev;
        fNext = rhs.fNext;
        fPrev->fNext = fNext->fPrev = this;
        rhs.fNext = rhs.fPrev = &rhs;
        return;
    }
    if (rhs.fNext == &rhs)
    {
        rhs.SwapWith(*this);
        return;
    }
    if (fNext == &rhs ) // rhs is next neighbour
    {
        if ( fPrev == &rhs )
            return;  // cycle of 2 pointers - no need to swap.
        std::swap(fPrev, fNext);
        std::swap(rhs.fPrev, rhs.fNext);
        std::swap(rhs.fPrev, fNext);
        std::swap(rhs.fPrev->fNext,fNext->fPrev);
    }
    else if ( fPrev == &rhs ) // rhs is prev neighbor
    {
        if ( fNext == &rhs )
            return;  // cycle of 2 pointers - no need to swap.
        std::swap( fPrev, fNext );
        std::swap( rhs.fNext, rhs.fPrev );
        std::swap( rhs.fNext, fPrev );
        std::swap( rhs.fNext->fPrev, fPrev->fNext );
    }
    else // not neighhbors
    {
        std::swap(fPrev, rhs.fPrev);
        std::swap(fNext, rhs.fNext);
        std::swap(fPrev->fNext, rhs.fPrev->fNext);
        std::swap(fNext->fPrev, rhs.fNext->fPrev);
    }


}
