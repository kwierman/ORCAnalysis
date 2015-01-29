#ifndef Protium_DoublyLinkedList_hh_
#define Protium_DoublyLinkedList_hh_

namespace Protium{
  namespace LinkedList{
  
    /**
      Basic Implementation of a linked list
    **/
    class DoubleLinkedNode{
      //! First Data Member and de facto insertion point
      mutable const DoubleLinkedNode* fPrev;
      //! Second Data Member
      mutable const DoubleLinkedNode* fNext;
      //! Count the number of nodes in the ``prev'' direction
      static unsigned int CountPrevCycle( const DoubleLinkedNode* pThis );
      //! Count the number of nodes in the ``next'' direction
      static unsigned int CountNextCycle( const DoubleLinkedNode* pThis );
      //! Look through the list in the ``prev'' direction and find out if p is in the list
      bool HasPrevNode( const DoubleLinkedNode* p ) const;
      //!Look through the list in the ``next'' direction and find out if p is in the list
      bool HasNextNode( const DoubleLinkedNode* p ) const;
    public:
      //! Default constructor assigns both pointees to itself
      DoubleLinkedNode(){
        fPrev = fNext = this;
      }
      //! Copy constructor inserts the copied node into the previous slot
      DoubleLinkedNode(const DoubleLinkedNode& rhs);

      const DoubleLinkedNode* GetPrev() {
        return fPrev;
      }
      const DoubleLinkedNode* GetNext() {
        return fNext;
      }

      bool MergeWith( DoubleLinkedNode & rhs );
      bool RemoveThis();
      void SwapWith(DoubleLinkedNode& rhs);
    };

  }
}

#endif//File Guardian




