#ifndef Protium_SmartPointer_StoragePolicies_hh_
#define Protium_SmartPointer_StoragePolicies_hh_

#include "Protium/Threads/Locker.hh"

namespace Protium{
    namespace SmartPointer{

    template <class T>
    class HeapStorage
    {
    public:
        typedef T* StoredType;      /// the type of the pointee_ object
        typedef T* InitPointerType; /// type used to declare OwnershipPolicy type.
        typedef T* PointerType;     /// type returned by operator->
        typedef T& ReferenceType;   /// type returned by operator*

        HeapStorage() : pointee_(Default())
        {}

        // The storage policy doesn't initialize the stored pointer
        //     which will be initialized by the OwnershipPolicy's Clone fn
        HeapStorage(const HeapStorage&) : pointee_(0)
        {}

        template <class U>
        HeapStorage(const HeapStorage<U>&) : pointee_(0)
        {}

        HeapStorage(const StoredType& p) : pointee_(p) {}

        PointerType operator->() const { return pointee_; }

        ReferenceType operator*() const { return *pointee_; }

        void Swap(HeapStorage& rhs)
        { std::swap(pointee_, rhs.pointee_); }

        // Accessors
        template <class F>
        friend typename HeapStorage<F>::PointerType GetImpl(const HeapStorage<F>& sp);

        template <class F>
        friend const typename HeapStorage<F>::StoredType& GetImplRef(const HeapStorage<F>& sp);

        template <class F>
        friend typename HeapStorage<F>::StoredType& GetImplRef(HeapStorage<F>& sp);

    protected:
        // Destroys the data stored
        // (Destruction might be taken over by the OwnershipPolicy)
        void Destroy()
        {
            if ( 0 != pointee_ )
            {
                pointee_->~T();
                ::free( pointee_ );
            }
        }

        // Default value to initialize the pointer
        static StoredType Default()
        { return 0; }

    private:
        // Data
        StoredType pointee_;
    };

    template <class T>
    inline typename HeapStorage<T>::PointerType GetImpl(const HeapStorage<T>& sp)
    { return sp.pointee_; }

    template <class T>
    inline const typename HeapStorage<T>::StoredType& GetImplRef(const HeapStorage<T>& sp)
    { return sp.pointee_; }

    template <class T>
    inline typename HeapStorage<T>::StoredType& GetImplRef(HeapStorage<T>& sp)
    { return sp.pointee_; }

    template <class T>
    class DefaultSPStorage
    {
    public:
        typedef T* StoredType;    // the type of the pointee_ object
        typedef T* InitPointerType; /// type used to declare OwnershipPolicy type.
        typedef T* PointerType;   // type returned by operator->
        typedef T& ReferenceType; // type returned by operator*

        DefaultSPStorage() : pointee_(Default())
        {}

        // The storage policy doesn't initialize the stored pointer
        //     which will be initialized by the OwnershipPolicy's Clone fn
        DefaultSPStorage(const DefaultSPStorage&) : pointee_(0)
        {}

        template <class U>
        DefaultSPStorage(const DefaultSPStorage<U>&) : pointee_(0)
        {}

        DefaultSPStorage(const StoredType& p) : pointee_(p) {}

        PointerType operator->() const { return pointee_; }

        ReferenceType operator*() const { return *pointee_; }

        void Swap(DefaultSPStorage& rhs)
        { std::swap(pointee_, rhs.pointee_); }

        // Accessors
        template <class F>
        friend typename DefaultSPStorage<F>::PointerType GetImpl(const DefaultSPStorage<F>& sp);

        template <class F>
        friend const typename DefaultSPStorage<F>::StoredType& GetImplRef(const DefaultSPStorage<F>& sp);

        template <class F>
        friend typename DefaultSPStorage<F>::StoredType& GetImplRef(DefaultSPStorage<F>& sp);

    protected:
        // Destroys the data stored
        // (Destruction might be taken over by the OwnershipPolicy)
        //
        // If your compiler gives you a warning in this area while
        // compiling the tests, it is on purpose, please ignore it.
        void Destroy()
        {
            delete pointee_;
        }

        // Default value to initialize the pointer
        static StoredType Default()
        { return 0; }

    private:
        // Data
        StoredType pointee_;
    };

    template <class T>
    inline typename DefaultSPStorage<T>::PointerType GetImpl(const DefaultSPStorage<T>& sp)
    { return sp.pointee_; }

    template <class T>
    inline const typename DefaultSPStorage<T>::StoredType& GetImplRef(const DefaultSPStorage<T>& sp)
    { return sp.pointee_; }

    template <class T>
    inline typename DefaultSPStorage<T>::StoredType& GetImplRef(DefaultSPStorage<T>& sp)
    { return sp.pointee_; }




    template <class T>
    class LockedStorage
    {
    public:

        typedef T* StoredType;           /// the type of the pointee_ object
        typedef T* InitPointerType;      /// type used to declare OwnershipPolicy type.
        typedef Protium::Threads::Locker< T > PointerType; /// type returned by operator->
        typedef T& ReferenceType;        /// type returned by operator*

        LockedStorage() : pointee_( Default() ) {}

        ~LockedStorage( void ) {}

        LockedStorage( const LockedStorage&) : pointee_( 0 ) {}

        LockedStorage( const StoredType & p ) : pointee_( p ) {}

        PointerType operator->()
        {
            return Protium::Threads::Locker< T >( pointee_ );
        }

        void Swap(LockedStorage& rhs)
        {
            std::swap( pointee_, rhs.pointee_ );
        }

        // Accessors
        template <class F>
        friend typename LockedStorage<F>::InitPointerType GetImpl(const LockedStorage<F>& sp);

        template <class F>
        friend const typename LockedStorage<F>::StoredType& GetImplRef(const LockedStorage<F>& sp);

        template <class F>
        friend typename LockedStorage<F>::StoredType& GetImplRef(LockedStorage<F>& sp);

    protected:
        // Destroys the data stored
        // (Destruction might be taken over by the OwnershipPolicy)
        void Destroy()
        {
            delete pointee_;
        }

        // Default value to initialize the pointer
        static StoredType Default()
        { return 0; }

    private:
        /// Dereference operator is not implemented.
        ReferenceType operator*();

        // Data
        StoredType pointee_;
    };

    template <class T>
    inline typename LockedStorage<T>::InitPointerType GetImpl(const LockedStorage<T>& sp)
    { return sp.pointee_; }

    template <class T>
    inline const typename LockedStorage<T>::StoredType& GetImplRef(const LockedStorage<T>& sp)
    { return sp.pointee_; }

    template <class T>
    inline typename LockedStorage<T>::StoredType& GetImplRef(LockedStorage<T>& sp)
    { return sp.pointee_; }


////////////////////////////////////////////////////////////////////////////////
///  \class ArrayStorage
///
///  \ingroup  SmartPointerStorageGroup
///  Implementation of the ArrayStorage used by SmartPtr
////////////////////////////////////////////////////////////////////////////////


    template <class T>
    class ArrayStorage
    {
    public:
        typedef T* StoredType;    // the type of the pointee_ object
        typedef T* InitPointerType; /// type used to declare OwnershipPolicy type.
        typedef T* PointerType;   // type returned by operator->
        typedef T& ReferenceType; // type returned by operator*

        ArrayStorage() : pointee_(Default())
        {}

        // The storage policy doesn't initialize the stored pointer
        //     which will be initialized by the OwnershipPolicy's Clone fn
        ArrayStorage(const ArrayStorage&) : pointee_(0)
        {}

        template <class U>
        ArrayStorage(const ArrayStorage<U>&) : pointee_(0)
        {}

        ArrayStorage(const StoredType& p) : pointee_(p) {}

        PointerType operator->() const { return pointee_; }

        ReferenceType operator*() const { return *pointee_; }

        void Swap(ArrayStorage& rhs)
        { std::swap(pointee_, rhs.pointee_); }

        // Accessors
        template <class F>
        friend typename ArrayStorage<F>::PointerType GetImpl(const ArrayStorage<F>& sp);

        template <class F>
        friend const typename ArrayStorage<F>::StoredType& GetImplRef(const ArrayStorage<F>& sp);

        template <class F>
        friend typename ArrayStorage<F>::StoredType& GetImplRef(ArrayStorage<F>& sp);

    protected:
        // Destroys the data stored
        // (Destruction might be taken over by the OwnershipPolicy)
        void Destroy()
        { delete [] pointee_; }

        // Default value to initialize the pointer
        static StoredType Default()
        { return 0; }

    private:
        // Data
        StoredType pointee_;
    };

    template <class T>
    inline typename ArrayStorage<T>::PointerType GetImpl(const ArrayStorage<T>& sp)
    { return sp.pointee_; }

    template <class T>
    inline const typename ArrayStorage<T>::StoredType& GetImplRef(const ArrayStorage<T>& sp)
    { return sp.pointee_; }

    template <class T>
    inline typename ArrayStorage<T>::StoredType& GetImplRef(ArrayStorage<T>& sp)
    { return sp.pointee_; }

    }
}


#endif

