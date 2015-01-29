#ifndef Protium_SmartPointer_Checking_hh_
#define Protium_SmartPointer_Checking_hh_

#include "Protium/SmartPointer/NullPointerException.hh"

#include "Protium/Design/Unused.hh"
#include "Protium/Design/Assert.hh"

namespace Protium{
	namespace SmartPointer{
        template <class Host>
        struct NoCheck{
            NoCheck(){}
            template <class Other>
            NoCheck(const NoCheck<Other>&) {}
            static void OnDefault(const Host&){}
            static void OnInit(const Host&){}
            static void OnDereference(const Host&){}
            static void Swap(NoCheck&){}
        };


        template <class Host>
        struct AssertCheck : public NoCheck<Host>{
            static void OnDereference(Host val){ 
                assert(val); 
                (void)val; 
            }
        };

        template <class Host>
        struct AssertCheckStrict : public AssertCheck<Host>{
            static void OnDefault(Host val)
            { assert(val); }

            static void OnInit(Host val)
            { assert(val); }

        };

    template <class Host>
    struct RejectNullStatic : public AssertCheckStrict<Host>{

        static void OnDefault(const Host&){
            static const bool DependedFalse = sizeof(Host*) == 0;
            PROTIUM_STATIC_ASSERT(DependedFalse, ERROR_This_Policy_Does_Not_Allow_Default_Initialization);
        }

        static void OnInit(const Host& val){ 
            if (!val) 
                throw NullPointerException(); 
        }

        static void OnDereference(const Host& val){ 
            if (!val) throw NullPointerException(); 
        }
    };


    template <class Host>
    struct RejectNull : public AssertCheckStrict<Host>{

        void OnDereference(Host val)
        { if (!val) throw NullPointerException(); }

        void OnDereference(Host val) const
        { if (!val) throw NullPointerException(); }

        void Swap(RejectNull&)
        {}
    };


    template <class Host>
    struct RejectNullStrict : public AssertCheckStrict<Host>{

        static void OnInit(Host val)
        { if (!val) throw NullPointerException(); }

        void OnDereference(Host val)
        { OnInit(val); }

        void OnDereference(Host val) const
        { OnInit(val); }

        void Swap(RejectNullStrict&)
        {}
    };
	}
}


#endif //File Guardian