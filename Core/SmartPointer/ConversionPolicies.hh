#ifndef ORCA_ConversionPolicies_hh_
#define ORCA_ConversionPolicies_hh_

namespace ORCA{
    namespace SmartPointer{
         struct AllowConversion{
            enum { allow = true };

            void Swap(AllowConversion&){}
        };
 
        struct DisallowConversion{
            DisallowConversion(){}

            DisallowConversion(const AllowConversion&){}

            enum { allow = false };

            void Swap(DisallowConversion&){}
        };           
    }
}

#endif //File Guardian

