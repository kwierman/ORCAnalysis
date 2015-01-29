#ifndef RefToValue_hh_
#define RefToValue_hh_

namespace Protium{


    namespace Design{

        //! Use to have local value that utilizes reference
        template<class T>
        class RefToValue{

            //! Reference to value
            T& fRef;

            //! No default constructor
            RefToValue();

            //! No copy assign
            RefToValue& operator=(const RefToValue&);
        public:

            //! Construct from reference
            RefToValue(T& ref) : fRef(ref) {}

            //! Copy constructor
            RefToValue(const RefToValue& rhs) : fRef(rhs.fRef) {}

            //! Access operator
            operator T& () const{return fRef;}
        };

        //! Helper construct method
        template <class T>
        inline RefToValue<T> ByRef(T& t){
            return RefToValue<T>(t);
        }    

    }
}


#endif