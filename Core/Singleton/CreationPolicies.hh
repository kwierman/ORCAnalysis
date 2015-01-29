#ifndef Protium_CreationPolicies_hh_
#define Protium_CreationPolicies_hh_

#include <malloc.h>

namespace Protium{
	namespace Singleton{
    
    	//! Helper Struct to create with new operator
    	//! \param T The class to be created
	    template <class T> 
		struct CreateNew{
			//! Creates instance
	        static T* Create() { return new T; }
	        
	        //! Destroys instance
	        static void Destroy(T* p) { delete p; }
	    };

	    //! Helper struct to create classes with a specific allocator
	    //! \param Alloc the allocator class to be used
		template< template<class> class Alloc>
	    struct CreateAlloc{
	    	//! Helper interior struct for allocating resources
	    	//! \param T the class to be created
	        template <class T>
	        struct Allocator{
	        	//! The allocator for all of these type classes
	            static Alloc<T> allocator;
	            //! Creates instance
	            static T* Create(){return new ( allocator.allocate(1) ) T;}

	            //! Destroys instance
	            static void Destroy(T* p){
	                p->~T();
	                allocator.deallocate(p,1);
	            }
	        };
	    };

	    //! Helper struct to create instances with malloc, and deallocate with free
	    template <class T> 
	    struct CreateMalloc{
	        static T* Create(){
	            void* p = malloc(sizeof(T));
	            if (!p) return NULL;
	            return new(p) T;
	        }
	        
	        static void Destroy(T* p){
	            p->~T();
	            free(p);
	        }
	    };

	    //!Helper struct to create static instances of classes
	    template <class T> 
	    struct CreateStatic{

	        //! Used to determine max type size
	        union MaxAlign
	        {
	            char t_[sizeof(T)];
	            short int shortInt_;
	            int int_;
	            long int longInt_;
	            float float_;
	            double double_;
	            long double longDouble_;
	            struct Test;
	            int Test::* pMember_;
	            int (Test::*pMemberFn_)(int);
	        };
	       
	        static T* Create(){
	            static MaxAlign staticMemory_;
	            return new(&staticMemory_) T;
	        }
	        
	        static void Destroy(T* p){
	            p->~T();
	        }
	    };
	}
}


#endif //File Guardian