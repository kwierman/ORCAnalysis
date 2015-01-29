#ifndef Protium_Vector_hh_
#define Protium_Vector_hh_

#include "Protium/Allocation/SmallObject.hh"
#include "Protium/Design/Assert.hh"

#include <vector>
#include <cmath>

namespace Protium{

	namespace LinearAlgebra{

		//! Implementation of vector to be used in Linear Algebra
		template<typename T, int n>
		class Vector;

		//! Helper template for generating subvectors of vectors
		template<typename T, int n>
		struct SubVector{
			Vector<T, n-1> Of(const Vector<T,n>& other, const int& i=0){
				Vector<T,n-1> temp;
				int index=0;
				for(int j=0; j<n;j++ ){
					if(j!=i)
						temp[index++] = T(other.At(j) );
				}
				return temp;
			}
		};

		//! Template specialization for lowest possible dimension subvector
		template<typename T>
		struct SubVector<T,2>{
			Vector<T, 1> Of(const Vector<T,2>& other, const int& i=0){
				Vector<T,1> temp;
				if(i==0)
					temp[0] = T(other.At(1) );
				else
					temp[0] = T(other.At(0) );
				return temp;
			}
		};

		//! Compiler error for vectors wishing too low in dimension
		template<typename T>
		struct SubVector<T,1>{
			Vector<T, 0> Of(const Vector<T,1>& other, const int& i=0){
				PROTIUM_STATIC_ASSERT(false, ERROR_NO_SUBVECTOR_OF_LENGTH_0() );
			}
		};

		template <typename T, int n=3>
		class Vector : public Protium::Allocation::DefaultSmallObject {

			//! Holds the components of the vector
			std::vector<T> fComponents;

		public:

            typedef T              value_type;
            typedef T*             iterator;
            typedef const T*       const_iterator;
            typedef T&             reference;
            typedef const T&       const_reference;
            typedef std::size_t    size_type;
            typedef std::ptrdiff_t difference_type;

			//! Initialization of vector components to 
			void Init(){
				PROTIUM_STATIC_ASSERT(n>0, ERROR_NO_SUBVECTOR_OF_LENGTH_0 );
				fComponents.clear();
				for(int i=0; i<n;i++)fComponents.push_back( T(1) );
			}

			/** Default Constructor.
				\param amplitude the amplitude of the vector
			**/
			Vector<T,n>(const T& amplitude=1) : Protium::Allocation::DefaultSmallObject() {
				
				Init();

				this->Normalize( amplitude );
			}

			/** Construct from an array. Copies contents.
				\param input An array of size n and type T
			**/
			Vector<T,n>(const T* input) : Protium::Allocation::DefaultSmallObject(){
				Init();
				for(int i=0; i<n; i++)
					fComponents[i] = T(input[i]);
			}

			/** Construct from STL Vector
			**/
			Vector<T,n>(const std::vector<T>& vec) : Protium::Allocation::DefaultSmallObject(){
				Init();
				for(int i=0; i<n;i++)
					fComponents[i] = T(vec[i] );
			}

			/** Copy constructor.
			**/
			Vector<T,n>(const Vector<T,n>& other) : Protium::Allocation::DefaultSmallObject(){
				Init();
				for(int i=0; i<n; i++){
					fComponents[i] =  T( other.At(i) );
				}
			}

			/** Default Destructor
			**/
			virtual ~Vector(){
				fComponents.clear();
			}

			/**
				Returns back the template integer parameter
			**/
			static int NDimensions() {
				return n;
			}

			/** Calculates the norm of the vector
				\return The square root of the square of this vector
			**/
		    virtual T Norm() const {
		      return sqrt( (*this)*(*this) );
		    }

		    /** Normalizes this vector to a constant
		    	\param norm The normalization constant
		    	\return The norm of the vector
		    **/
		    virtual T Normalize(const T& norm){
		    	T old_norm = this->Norm();
		    	if(old_norm == norm) return norm;
		    	if(old_norm ==0) return 0;
		    	for(int i=0 ;i<n; i++ )
		    		fComponents[i] *= (norm/old_norm);
		    	return this->Norm();
		    }

		    /** Calculates and returns a vector of one less dimension
		    	\param i The index of the element to be left out
		    	\return A vector of one less dimension
		    **/
		   
		    Vector<T,n-1> GetSubVector(const int& i){
		    	SubVector<T,n> helper;
		    	return helper.Of( (*this), i ); 
		    }
		    
		    /** Access operator.
		    **/
		    T& operator[](const int& index){
		    	return fComponents[index];
		    }

		    /** Const access operator
		    **/
		    const T& At(const int& index) const{
		    	return fComponents.at(index);
		    }

		    /** Increment operator
		    	\param rhs Vector to increment by (increments each element by it's corresponding one)
		    **/
			Vector<T,n>& operator+=(const Vector<T,n>& rhs) {
				//PROTIUM_STATIC_ASSERT(n == m,"Vector Dimensions Must Match");
				for(int i=0; i<n;i++)
					 (*this)[i]  += rhs.At(i);
	    		return *this;
	  		}

	  		/** Operator only defined for multiplying by another type
	  		**/
			Vector<T,n>& operator*=(const T& rhs) {
				//PROTIUM_STATIC_ASSERT(n == m,"Vector Dimensions Must Match");
				for(int i=0; i<n;i++)
					 (*this)[i] *=rhs;
	    		return *this;
	  		}

	  		/** Decrement operator
	  		**/
			Vector<T,n>& operator-=(const Vector<T,n>& rhs) {
				//PROTIUM_STATIC_ASSERT(n == m,"Vector Dimensions Must Match");
				for(int i=0; i<n;i++)
					 (*this)[i] -=rhs.At(i);
	    		return *this;
	  		}

	  		/** Addition operator
	  		**/
	  		const Vector<T,n> operator+(const Vector<T,n>& rhs) const {
	    		return Vector<T,n>(*this) += rhs;
	  		}

	  		/** Subtraction operator
	  		**/
	  		const Vector<T,n> operator-(const Vector<T,n>& rhs) const {
	    		return Vector<T,n>(*this) -= rhs;
	  		}

	  		/** Multiplication Operator
	  			\param rhs A vector of same type and size
	  			\return constant of vector type
	  		**/
	  		const T operator*(const Vector<T,n>& rhs) const {
	  			T ret=0;
				//PROTIUM_STATIC_ASSERT(n == m,"Vector Dimensions Must Match");
				for(int i=0; i<n;i++){
					ret+= (this->At(i)) * (rhs.At(i) );
				}

	    		return ret;
	  		}

	  		/** Multiplication Operator
	  			\param rhs Of vector type
	  			\return Vector of same size and type as current one
	  		**/
			const Vector<T,n> operator*(const T& rhs) const {
	  			return Vector<T,n>(*this) *=rhs;
	  		}

	  		/** Assignment Operator
	  		**/
	  		Vector<T,n>& operator=( const Vector<T,n>& rhs ){
      			for(int i=0; i<n;i++) fComponents.at(i) = T(rhs.At(i) );
      			return *this;
  			}

	  		/** Comparison operator
	  		**/
	  		bool operator==( const Vector<T,n> &rhs) const {
	  			bool ret=true;
				//PROTIUM_STATIC_ASSERT(n == m,"Vector Dimensions Must Match");
	  			for(int i=0; i<n;i++)
	  				ret &= ( this->At(i) == rhs.At(i) );
	  			return ret;
	  		}

	  		/** Negative comparison operator
	  		**/
		  	bool operator!=( const Vector<T,n> &rhs) const {
	    		return !(*this == rhs);
	  		}

	  		//! Constructs a unit vector in the given dimension
	  		static Vector<T,n> UnitVector(int dim=0){
	  			Vector<T,n> vec;
	  			for(int i=0;i<n;i++)
	  				if(i==dim)
	  					vec[i]=T(1);
	  				else
	  					vec[i]=T(0);
	  			return vec;
	  		}

		};

		//! Convenience Typedef of a double twovector
		typedef Vector<double, 2> TwoVector;
		//! Convenience typedef of a double three vector
		typedef Vector<double, 3> ThreeVector;
		//! Convenience typedef of a double four vector
		typedef Vector<double, 4> FourVector;
		typedef Vector<double, 5> FiveVector;
		typedef Vector<double, 6> SixVector;
		typedef Vector<double, 7> SevenVector;
		typedef Vector<double, 8> EightVector;
		typedef Vector<double, 9> NineVector;
		typedef Vector<double, 10> TenVector;
		typedef Vector<double, 11> ElevenVector;
		typedef Vector<double, 12> TwelveVector;
		typedef Vector<double, 13> ThirteenVector;
		typedef Vector<double, 14> FourteenVector;
		typedef Vector<double, 15> FifteenVector;
	}
}


#endif //File Guardian

