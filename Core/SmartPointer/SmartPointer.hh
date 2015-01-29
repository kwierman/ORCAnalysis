#ifndef Protium_SmartPointer_h_
#define Protium_SmartPointer_h_

#include "Protium/SmartPointer/CheckingPolicies.hh"
#include "Protium/SmartPointer/ConversionPolicies.hh"
#include "Protium/SmartPointer/OwnershipPolicy.hh"
#include "Protium/SmartPointer/StoragePolicies.hh"

#include "Protium/Design/Types.hh"
#include "Protium/Design/RefToValue.hh"

namespace Protium{

    namespace SmartPointer{



    template
    <
        typename T,
        template <class> class OwnershipPolicy = ReferenceCounted,
        class ConversionPolicy = DisallowConversion,
        template <class> class CheckingPolicy = AssertCheck,
        template <class> class StoragePolicy = DefaultSPStorage
     >
     class SmartPtr;

    template
    <
        typename T,
        template <class> class OwnershipPolicy = ReferenceCounted,
        class ConversionPolicy = DisallowConversion,
        template <class> class CheckingPolicy = AssertCheck,
        template <class> class StoragePolicy = DefaultSPStorage
    >
    struct SmartPtrDef
    {
        typedef SmartPtr
        <
            T,
            OwnershipPolicy,
            ConversionPolicy,
            CheckingPolicy,
            StoragePolicy
        >
        type;
    };

    template
    <
        typename T,
        template <class> class OwnershipPolicy,
        class ConversionPolicy,
        template <class> class CheckingPolicy,
        template <class> class StoragePolicy
    >
    class SmartPtr
        : public StoragePolicy<T>
        , public OwnershipPolicy<typename StoragePolicy<T>::InitPointerType>
        , public CheckingPolicy<typename StoragePolicy<T>::StoredType>
        , public ConversionPolicy
    {
        typedef StoragePolicy<T> SP;
        typedef OwnershipPolicy<typename StoragePolicy<T>::InitPointerType> OP;
        typedef CheckingPolicy<typename StoragePolicy<T>::StoredType> KP;
        typedef ConversionPolicy CP;

    public:

        typedef typename SP::PointerType PointerType;
        typedef typename SP::StoredType StoredType;
        typedef typename SP::ReferenceType ReferenceType;
        typedef const PointerType ConstPointerType;
        typedef const ReferenceType ConstReferenceType;

        typedef typename Protium::Design::Select<OP::destructiveCopy,SmartPtr, const SmartPtr>::Result
                CopyArg;

    private:
        struct NeverMatched {};


        typedef const StoredType& ImplicitArg;
        typedef const NeverMatched& ExplicitArg;

    public:
        //Default constructor uses the checking policy to check if default is OK
        SmartPtr()
        {
            KP::OnDefault(GetImpl(*this));
        }

        //
        explicit
        SmartPtr(ExplicitArg p) : SP(p)
        {
            KP::OnInit(GetImpl(*this));
        }

        SmartPtr(ImplicitArg p) : SP(p)
        {
            KP::OnInit(GetImpl(*this));
        }

        SmartPtr(CopyArg& rhs) : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
        {
            GetImplRef(*this) = OP::Clone(GetImplRef(rhs));
        }

        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        SmartPtr(const SmartPtr<T1, OP1, CP1, KP1, SP1 >& rhs)
        : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
        { GetImplRef(*this) = OP::Clone(GetImplRef(rhs)); }

        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        SmartPtr(SmartPtr<T1, OP1, CP1, KP1, SP1 >& rhs)
        : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
        {
            GetImplRef(*this) = OP::Clone(GetImplRef(rhs));
        }

        SmartPtr( Protium::Design::RefToValue<SmartPtr> rhs)
        : SP(rhs), OP(rhs), KP(rhs), CP(rhs)
        {}

        operator Protium::Design::RefToValue<SmartPtr>()
        { return Protium::Design::RefToValue<SmartPtr>(*this); }

        SmartPtr& operator=(CopyArg& rhs)
        {
            SmartPtr temp(rhs);
            temp.Swap(*this);
            return *this;
        }

        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        SmartPtr& operator=(const SmartPtr<T1, OP1, CP1, KP1, SP1 >& rhs)
        {
            SmartPtr temp(rhs);
            temp.Swap(*this);
            return *this;
        }

        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        SmartPtr& operator=(SmartPtr<T1, OP1, CP1, KP1, SP1 >& rhs)
        {
            SmartPtr temp(rhs);
            temp.Swap(*this);
            return *this;
        }

        void Swap(SmartPtr& rhs)
        {
            OP::Swap(rhs);
            CP::Swap(rhs);
            KP::Swap(rhs);
            SP::Swap(rhs);
        }

        ~SmartPtr()
        {
            if (OP::Release(GetImpl(*static_cast<SP*>(this))))
            {
                SP::Destroy();
            }
        }


        // old non standard in class definition of friends
        friend inline void Release(SmartPtr& sp, typename SP::StoredType& p)
        {
            p = GetImplRef(sp);
            GetImplRef(sp) = SP::Default();
        }

        friend inline void Reset(SmartPtr& sp, typename SP::StoredType p)
        { SmartPtr(p).Swap(sp); }


        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        bool Merge( SmartPtr< T1, OP1, CP1, KP1, SP1 > & rhs )
        {
            if ( GetImpl( *this ) != GetImpl( rhs ) )
            {
                return false;
            }
            return OP::template Merge( rhs );
        }

        PointerType operator->()
        {
            KP::OnDereference(GetImplRef(*this));
            return SP::operator->();
        }

        ConstPointerType operator->() const
        {
            KP::OnDereference(GetImplRef(*this));
            return SP::operator->();
        }

        ReferenceType operator*()
        {
            KP::OnDereference(GetImplRef(*this));
            return SP::operator*();
        }

        ConstReferenceType operator*() const
        {
            KP::OnDereference(GetImplRef(*this));
            return SP::operator*();
        }

        bool operator!() const // Enables "if (!sp) ..."
        { return GetImpl(*this) == 0; }

        static inline T * GetPointer( const SmartPtr & sp )
        { return GetImpl( sp ); }

        // Ambiguity buster
        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        bool operator==(const SmartPtr<T1, OP1, CP1, KP1, SP1 >& rhs) const
        { return GetImpl(*this) == GetImpl(rhs); }

        // Ambiguity buster
        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        bool operator!=(const SmartPtr<T1, OP1, CP1, KP1, SP1 >& rhs) const
        { return !(*this == rhs); }

        // Ambiguity buster
        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        bool operator<(const SmartPtr<T1, OP1, CP1, KP1, SP1 >& rhs) const
        { return GetImpl(*this) < GetImpl(rhs); }

        // Ambiguity buster
        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        inline bool operator > ( const SmartPtr< T1, OP1, CP1, KP1, SP1 > & rhs )
        {
            return ( GetImpl( rhs ) < GetImpl( *this ) );
        }

        // Ambiguity buster
        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        inline bool operator <= ( const SmartPtr< T1, OP1, CP1, KP1, SP1 > & rhs )
        {
            return !( GetImpl( rhs ) < GetImpl( *this ) );
        }

        // Ambiguity buster
        template
        <
            typename T1,
            template <class> class OP1,
            class CP1,
            template <class> class KP1,
            template <class> class SP1
        >
        inline bool operator >= ( const SmartPtr< T1, OP1, CP1, KP1, SP1 > & rhs )
        {
            return !( GetImpl( *this ) < GetImpl( rhs ) );
        }

    private:
        // Helper for enabling 'if (sp)'
        struct Tester
        {
            Tester(int) {}
            void dummy() {}
        };

        typedef void (Tester::*unspecified_boolean_type_)();

        typedef typename Protium::Design::Select<CP::allow, Tester, unspecified_boolean_type_>::Result
            unspecified_boolean_type;

    public:
        // enable 'if (sp)'
        operator unspecified_boolean_type() const
        {
            return !*this ? 0 : &Tester::dummy;
        }

    private:
        // Helper for disallowing automatic conversion
        struct Insipid
        {
            Insipid(PointerType) {}
        };

        typedef typename Protium::Design::Select<CP::allow, PointerType, Insipid>::Result
            AutomaticConversionResult;

    public:
        operator AutomaticConversionResult() const
        { return GetImpl(*this); }
    };

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator==(const SmartPtr<T, OP, CP, KP, SP >& lhs,
        U* rhs)
    { return GetImpl(lhs) == rhs; }

////////////////////////////////////////////////////////////////////////////////
///  operator== for lhs = raw pointer, rhs = SmartPtr
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator==(U* lhs,
        const SmartPtr<T, OP, CP, KP, SP >& rhs)
    { return rhs == lhs; }

////////////////////////////////////////////////////////////////////////////////
///  operator!= for lhs = SmartPtr, rhs = raw pointer
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator!=(const SmartPtr<T, OP, CP, KP, SP >& lhs,
        U* rhs)
    { return !(lhs == rhs); }

////////////////////////////////////////////////////////////////////////////////
///  operator!= for lhs = raw pointer, rhs = SmartPtr
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator!=(U* lhs,
        const SmartPtr<T, OP, CP, KP, SP >& rhs)
    { return rhs != lhs; }

////////////////////////////////////////////////////////////////////////////////
///  operator< for lhs = SmartPtr, rhs = raw pointer
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator<(const SmartPtr<T, OP, CP, KP, SP >& lhs,
        U* rhs)
    {
        return ( GetImpl( lhs ) < rhs );
    }

////////////////////////////////////////////////////////////////////////////////
///  operator< for lhs = raw pointer, rhs = SmartPtr
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator<(U* lhs,
        const SmartPtr<T, OP, CP, KP, SP >& rhs)
    {
        return ( GetImpl( rhs ) < lhs );
    }

////////////////////////////////////////////////////////////////////////////////
//  operator> for lhs = SmartPtr, rhs = raw pointer
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator>(const SmartPtr<T, OP, CP, KP, SP >& lhs,
        U* rhs)
    { return rhs < lhs; }

////////////////////////////////////////////////////////////////////////////////
///  operator> for lhs = raw pointer, rhs = SmartPtr
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator>(U* lhs,
        const SmartPtr<T, OP, CP, KP, SP >& rhs)
    { return rhs < lhs; }

////////////////////////////////////////////////////////////////////////////////
///  operator<= for lhs = SmartPtr, rhs = raw pointer
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator<=(const SmartPtr<T, OP, CP, KP, SP >& lhs,
        U* rhs)
    { return !(rhs < lhs); }

////////////////////////////////////////////////////////////////////////////////
///  operator<= for lhs = raw pointer, rhs = SmartPtr
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator<=(U* lhs,
        const SmartPtr<T, OP, CP, KP, SP >& rhs)
    { return !(rhs < lhs); }

////////////////////////////////////////////////////////////////////////////////
///  operator>= for lhs = SmartPtr, rhs = raw pointer
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator>=(const SmartPtr<T, OP, CP, KP, SP >& lhs,
        U* rhs)
    { return !(lhs < rhs); }

////////////////////////////////////////////////////////////////////////////////
///  operator>= for lhs = raw pointer, rhs = SmartPtr
///  \ingroup SmartPointerGroup
////////////////////////////////////////////////////////////////////////////////

    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP,
        typename U
    >
    inline bool operator>=(U* lhs,
        const SmartPtr<T, OP, CP, KP, SP >& rhs)
    { return !(lhs < rhs); }






    }
}

namespace std
{
    template
    <
        typename T,
        template <class> class OP,
        class CP,
        template <class> class KP,
        template <class> class SP
    >
    struct less< Protium::SmartPointer::SmartPtr<T, OP, CP, KP, SP > >
        : public binary_function<Protium::SmartPointer::SmartPtr<T, OP, CP, KP, SP >,
            Protium::SmartPointer::SmartPtr<T, OP, CP, KP, SP >, bool>
    {
        bool operator()(const Protium::SmartPointer::SmartPtr<T, OP, CP, KP, SP >& lhs,
            const Protium::SmartPointer::SmartPtr<T, OP, CP, KP, SP >& rhs) const
        { return less<T*>()(GetImpl(lhs), GetImpl(rhs)); }
    };
}


#endif //File Guardian
//END FILE PROTIUM DEFINITION






