#ifndef Protium_OwnershipPolicy_h_
#define Protium_OwnershipPolicy_h_

#include "Protium/LinkedList/DoublyLinkedList.hh"

#include "Protium/Allocation/SmallObject.hh"
#include "Protium/Threads/Mutex.hh"



namespace Protium{
	namespace SmartPointer{

        template<class Host>
        class ReferenceCounted{
            unsigned* fCount;
        public:
            ReferenceCounted() : fCount( static_cast<unsigned*>(Protium::Allocation::SmallObject<>::operator new( sizeof(unsigned) ) ) ) {
                assert(fCount!=0);
                *fCount = 1;
            }
            ReferenceCounted(const ReferenceCounted& other) : fCount(other.fCount) {}
            unsigned* getCount(){return fCount;}

            template<typename Other>
            ReferenceCounted(const ReferenceCounted<Other>& other) : fCount(other.getCount){}
            Host Clone(const Host& val){
                ++(*fCount);
                return val;
            }
            bool Release(const Host& val){
                if( !(--(*fCount) ) ){
                    Protium::Allocation::SmallObject<>::operator delete(fCount, sizeof(unsigned) );
                    return true;
                }
                return false;
            }
            void Swap(ReferenceCounted& rhs){ 
                std::swap(fCount, rhs.fCount); 
            }
                    enum { destructiveCopy = false };
        };


////////////////////////////////////////////////////////////////////////////////
///  \struct RefCountedMT
///
///  \ingroup  SmartPointerOwnershipGroup
///  Implementation of the OwnershipPolicy used by SmartPtr
///  Implements external reference counting for multithreaded programs
///  Policy Usage: RefCountedMTAdj<ThreadingModel>::RefCountedMT
///
///  \par Warning
///  There could be a race condition, see bug "Race condition in RefCountedMTAdj::Release"
///  http://sourceforge.net/tracker/index.php?func=detail&aid=1408845&group_id=29557&atid=396644
///  As stated in bug 1408845, the Release function is not thread safe if a
///  SmartPtr copy-constructor tries to copy the last pointer to an object in
///  one thread, while the destructor is acting on the last pointer in another
///  thread.  The existence of a race between a copy-constructor and destructor
///  implies a design flaw at a higher level.  That race condition must be
///  fixed at a higher design level, and no change to this class could fix it.
////////////////////////////////////////////////////////////////////////////////

    template <template <class, class> class ThreadingModel,
              class MX = Protium::Threads::Mutex >
    struct RefCountedMTAdj
    {
        template <class P>
        class RefCountedMT : public ThreadingModel< RefCountedMT<P>, MX >
        {
            typedef ThreadingModel< RefCountedMT<P>, MX > base_type;
            typedef typename base_type::IntType       CountType;
            typedef volatile CountType               *CountPtrType;

        public:
            RefCountedMT()
            {
                pCount_ = static_cast<CountPtrType>(
                    Protium::Allocation::SmallObject<Protium::Threads::InstanceLocked>::operator new(
                        sizeof(*pCount_)));
                assert(pCount_);
                //*pCount_ = 1;
                ThreadingModel<RefCountedMT, MX>::AtomicAssign(*pCount_, 1);
            }

            RefCountedMT(const RefCountedMT& rhs)
            : pCount_(rhs.pCount_)
            {}

            //MWCW lacks template friends, hence the following kludge
            template <typename P1>
            RefCountedMT(const RefCountedMT<P1>& rhs)
            : pCount_(reinterpret_cast<const RefCountedMT<P>&>(rhs).pCount_)
            {}

            P Clone(const P& val)
            {
                ThreadingModel<RefCountedMT, MX>::AtomicIncrement(*pCount_);
                return val;
            }

            bool Release(const P&)
            {
                bool isZero = false;
                ThreadingModel< RefCountedMT, MX >::AtomicDecrement( *pCount_, 0, isZero );
                if ( isZero )
                {
                    Protium::Allocation::SmallObject<Protium::Threads::InstanceLocked>::operator delete(
                        const_cast<CountType *>(pCount_),
                        sizeof(*pCount_));
                    return true;
                }
                return false;
            }

            void Swap(RefCountedMT& rhs)
            { std::swap(pCount_, rhs.pCount_); }

            enum { destructiveCopy = false };

        private:
            // Data
            CountPtrType pCount_;
        };
    };

////////////////////////////////////////////////////////////////////////////////
///  \class COMRefCounted
///
///  \ingroup  SmartPointerOwnershipGroup
///  Implementation of the OwnershipPolicy used by SmartPtr
///  Adapts COM intrusive reference counting to OwnershipPolicy-specific syntax
////////////////////////////////////////////////////////////////////////////////

    template <class P>
    class COMRefCounted
    {
    public:
        COMRefCounted()
        {}

        template <class U>
        COMRefCounted(const COMRefCounted<U>&)
        {}

        static P Clone(const P& val)
        {
            if(val!=0)
               val->AddRef();
            return val;
        }

        static bool Release(const P& val)
        {
            if(val!=0)
                val->Release();
            return false;
        }

        enum { destructiveCopy = false };

        static void Swap(COMRefCounted&)
        {}
    };

////////////////////////////////////////////////////////////////////////////////
///  \struct DeepCopy
///
///  \ingroup  SmartPointerOwnershipGroup
///  Implementation of the OwnershipPolicy used by SmartPtr
///  Implements deep copy semantics, assumes existence of a Clone() member
///  function of the Host type
////////////////////////////////////////////////////////////////////////////////

    template <class P>
    struct DeepCopy
    {
        DeepCopy()
        {}

        template <class P1>
        DeepCopy(const DeepCopy<P1>&)
        {}

        static P Clone(const P& val)
        { return val->Clone(); }

        static bool Release(const P&)
        { return true; }

        static void Swap(DeepCopy&)
        {}

        enum { destructiveCopy = false };
    };

////////////////////////////////////////////////////////////////////////////////
///  \class RefLinked
///
///  \ingroup  SmartPointerOwnershipGroup
///  Implementation of the OwnershipPolicy used by SmartPtr
///  Implements reference linking
////////////////////////////////////////////////////////////////////////////////



    template <class P>
    class RefLinked : public Protium::LinkedList::DoubleLinkedNode
    {

    public:
        RefLinked()
        {}

        template <class P1>
        RefLinked(const RefLinked<P1>& rhs)
        : Protium::LinkedList::DoubleLinkedNode(rhs)
        {}

        static P Clone(const P& val)
        { return val; }

        bool Release(const P&)
        { return Protium::LinkedList::DoubleLinkedNode::RemoveThis(); }

        template < class P1 >
        bool Merge( RefLinked< P1 > & rhs )
        {
            return Protium::LinkedList::DoubleLinkedNode::MergeWith( rhs );
        }
    };

////////////////////////////////////////////////////////////////////////////////
///  \class DestructiveCopy
///
///  \ingroup  SmartPointerOwnershipGroup
///  Implementation of the OwnershipPolicy used by SmartPtr
///  Implements destructive copy semantics (a la std::auto_ptr)
////////////////////////////////////////////////////////////////////////////////

    template <class P>
    class DestructiveCopy
    {
    public:
        DestructiveCopy()
        {}

        template <class P1>
        DestructiveCopy(const DestructiveCopy<P1>&)
        {}

        template <class P1>
        static P Clone(P1& val)
        {
            P result(val);
            val = P1();
            return result;
        }

        static bool Release(const P&)
        { return true; }

        static void Swap(DestructiveCopy&)
        {}

        enum { destructiveCopy = true };
    };

////////////////////////////////////////////////////////////////////////////////
///  \class NoCopy
///
///  \ingroup  SmartPointerOwnershipGroup
///  Implementation of the OwnershipPolicy used by SmartPtr
///  Implements a policy that doesn't allow copying objects
////////////////////////////////////////////////////////////////////////////////

    template <class P>
    class NoCopy
    {
    public:
        NoCopy()
        {}

        template <class P1>
        NoCopy(const NoCopy<P1>&)
        {}

        static P Clone(const P&)
        {
            // Make it depended on template parameter
            static const bool DependedFalse = sizeof(P*) == 0;

            PROTIUM_STATIC_ASSERT(DependedFalse, This_Policy_Disallows_Value_Copying);
        }

        static bool Release(const P&)
        { return true; }

        static void Swap(NoCopy&)
        {}

        enum { destructiveCopy = false };
    };


		
	}
}

#endif //Protium_OwnershipPolicy_h_

