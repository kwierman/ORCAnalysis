#ifndef Protium_Types_hh_
#define Protium_Types_hh_

namespace Protium{

	namespace Design{

		//! Emmpty struct. To be used as header or footer.
	    struct NullType{};
	    //! Empty class. To be used as header or footer.
	    class EmptyType {};
	    
	    //! Empty types are always equal
	    inline bool operator==(const EmptyType&, const EmptyType&){return true;}   

		//! Empty types are always equal
		inline bool operator<(const EmptyType&, const EmptyType&){return false;}

		//! Empty types are always equal
	    inline bool operator>(const EmptyType&, const EmptyType&){return false;}

	    //! Maps type to integer
	    template <int v>
	    struct IntType{
	        enum { value = v };
	    };
	    
	    //! Maps type to another type
	    template <typename T>
	    struct TypeType{
	        typedef T OType;
	    };

		template <bool toggle, typename A, typename B>
		struct Select{
			typedef A Result;
		};

		template <typename A, typename B>
		struct Select<false, A, B>{
			typedef B Result;
		};

	    //! Maps to types to a new type
	    template <typename T, typename U>
	    struct SameType
	    {
	        enum { value = false };
	    };
	    
	    //! Specialization of SameType for identical types
	    template <typename T>
	    struct SameType<T,T>
	    {
	        enum { value = true };
	    };

	    //! General list of types. Points to Head and Tail
	    /*!
	    	Use like `Typelist<MyClass, NullType>;`
	    */
	    template <class H, class T>
	    struct Typelist{
	       typedef H Head;
	       typedef T Tail;
	    };        

	    //! Empty Astract Index class. Never Use directly.
        template <class TList, class T> 
        struct IndexOf;
        
        //! The Last element in a list is always NUllType at position -1
        template <class T>
        struct IndexOf<NullType, T>
        {
            enum { value = -1 };
        };
        
        //! Every Typelist has index of 0
        template <class T, class Tail>
        struct IndexOf<Typelist<T, Tail>, T>
        {
            enum { value = 0 };
        };
        
        //! Every item in typelist has index (typelist) + 1
        template <class Head, class Tail, class T>
        struct IndexOf<Typelist<Head, Tail>, T>
        {
        private:
            enum { temp = IndexOf<Tail, T>::value };
        public:
            enum { value = (temp == -1 ? -1 : 1 + temp) };
        };


        //! Sequence of types. 
	    template<	class T1=NullType,
	    			class T2=NullType,
	    			class T3=NullType,
	    			class T4=NullType,
	    			class T5=NullType,
	        		class T6=NullType,
	    			class T7=NullType,
	    			class T8=NullType,
	    			class T9=NullType,
	    			class T10=NullType,
			        class T11=NullType,
	      			class T12=NullType,
	       			class T13=NullType,
	       			class T14=NullType,
	       			class T15=NullType,
	       			class T16=NullType,
	    			class T17=NullType,
	    			class T18=NullType,
	    			class T19=NullType,
	    			class T20=NullType
	  			>
	    struct Sequence{
	    private:
	        typedef typename Sequence<	T2,
        			 					T3,
        			 					T4,
        			 					T5,
        			 					T6,
        			 					T7,
        			 					T8,
        			 					T9,
        			 					T10,
        			 					T11,
										T12,
										T13,
										T14,
										T15,
										T16,
										T17,
										T18,
										T19,
                             			T20	>::Type TailResult;
	    public:
	        typedef Typelist<T1, TailResult> Type;
	    };
	        
	    //! Specialization of empty sequence
	    template<>
	    struct Sequence<>{
	        typedef NullType Type;
	    };


	}
}

#endif //File Guardian