#ifndef Protium_Functional_hh_
#define Protium_Functional_hh_

#include <map>

namespace Protium{

	namespace Containers{


		/** \class Functional1D
			\brief A base template to deal with the minutia of 
		**/
		template<typename XType=double, typename YType=double>
		class Functional1D{
		protected:

			typedef std::map<XType, YType> DataType;
			DataType fData;

			typedef typename DataType::iterator DataIt;

			typedef XType              value_type;
            typedef XType*             iterator;
            typedef const XType*       const_iterator;
            typedef XType&             reference;
            typedef const XType&       const_reference;
            typedef std::size_t        size_type;
            typedef std::ptrdiff_t     difference_type;

		public:

			Functional1D(){}


			virtual YType At(const XType& val) const{
				return (*fData.lower_bound(val)).second;
			}
			virtual YType& RefAt(const XType& val){
				return (*fData.lower_bound(val)).second;
			}

			Functional1D(const Functional1D& other){
				this->fData = other.fData;
			}

			virtual ~Functional1D(){}//nothin to see here, folks

			virtual Functional1D& operator*=(const YType& other){
				for(DataIt it = fData.begin(); it!= fData.end(); ++it){
					it->second *= other;
				}
			}
			virtual Functional1D& operator/=(const YType& other){
				for(DataIt it = fData.begin(); it!= fData.end(); ++it){
					it->second /= other;
				}
			}
			virtual Functional1D& operator*(const YType& other){
				return Functional1D<XType, YType>(*this)*=other;
			}
			virtual Functional1D& operator/(const YType& other){
				return Functional1D<XType, YType>(*this)/=other;
			}

			virtual Functional1D& operator*=(const Functional1D& other){
				for(DataIt it = fData.begin(); it!= fData.end(); ++it)
					if(other.At( (it->first)) ){
						it->second *= other.At( (it->first) );
					}
				return *this;
			}
			virtual Functional1D& operator/=(const Functional1D& other){
				for(DataIt it = fData.begin(); it!= fData.end(); ++it)
					if(other.At( (it->first)) ){
						it->second /= other.At( (it->first) );
					}
				return *this;
			}
			virtual Functional1D& operator+=(const Functional1D& other){
				for(DataIt it = fData.begin(); it!= fData.end(); ++it)
					if(other.At( (it->first)) ){
						it->second += other.At( (it->first) );
					}
				return *this;
			}
			virtual Functional1D& operator-=(const Functional1D& other){				
				for(DataIt it = fData.begin(); it!= fData.end(); ++it)
					if(other.At( (it->first)) ){
						it->second -= other.At( (it->first) );
					}
				return *this;
			}

			/*
			virtual bool operator==(const Functional1D& other) const{
				for(DataIt it = fData.begin(); it!= fData.end(); ++it)
					if(other.At( (it->first)) != it->second ){
						return false;
					}
				return true;
			}
			virtual Functional1D& operator!=(const Functional1D& other) const{
				return ~(*this == other);
			}
			*/

			virtual Functional1D& operator*(const Functional1D& other){
				return Functional1D(*this)*=other;
			}
			virtual Functional1D& operator/(const Functional1D& other){
				return Functional1D(*this)/=other;
			}
			virtual Functional1D& operator+(const Functional1D& other){
				return Functional1D(*this)+=other;
			}
			virtual Functional1D& operator-(const Functional1D& other){
				return Functional1D(*this)-=other;
			}

			//Assignent operator needs to be overloaded in a higher frame of reference
			virtual Functional1D& operator=(const Functional1D& other){
				this->fData.clear();
				fData = other.fData;
				return *this;
			}

		};




		template<typename XType, typename YType, typename ZType>
		class Functional2D : Functional1D< XType, Functional1D<YType, ZType> >{
		public:
			virtual ZType At(const XType& xVal, const YType& yVal) const{
				return Functional1D< XType, Functional1D<YType, ZType> >::fData.lower_bound(xVal).At(yVal);
			}
			virtual ZType& RefAt(const XType& xVal, const YType& yVal){
				return Functional1D< XType, Functional1D<YType, ZType> >::fData.lower_bound(xVal).RefAt(yVal);
			}
		};


		template<typename XType, typename YType, typename ZType, typename TType>
		class Functional3D : Functional1D<XType, Functional2D<YType, ZType, TType> >{	
		public:
			virtual TType At(const XType& xVal, const YType& yVal, const ZType& zVal) const{
				return Functional1D<XType, Functional2D<YType, ZType, TType> >::fData.lower_bound(xVal).At(yVal).At(zVal);
			}
			virtual TType& RefAt(const XType& xVal, const YType& yVal, const ZType& zVal){
				return Functional1D<XType, Functional2D<YType, ZType, TType> >::fData.lower_bound(xVal).RefAt(yVal).RefAt(zVal);
			}
		};

		template<typename XType, typename YType, typename ZType, typename TType, typename AType>
		class Functional4D : Functional1D<XType, Functional3D<YType, ZType, TType, AType> >{
		public:
			virtual AType At(const XType& xVal, const YType& yVal, const ZType& zVal, const TType& tVal) const{
				return Functional1D<XType, Functional3D<YType, ZType, TType, AType> >::fData.lower_bound(xVal).At(yVal).At(zVal).At(tVal);
			}
			virtual AType& RefAt(const XType& xVal, const YType& yVal, const ZType& zVal, const TType& tVal){
				return Functional1D<XType, Functional3D<YType, ZType, TType, AType> >::fData.lower_bound(xVal).RefAt(yVal).RefAt(zVal).RefAt(tVal);
			}
		};

		typedef Functional1D<double, double> Func1D;
		typedef Functional2D<double, double, double> Func2D;
		typedef Functional3D<double, double, double, double> Func3D;
	}
}

#endif //File Guardian