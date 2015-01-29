#ifndef Protium_PhysicalValue_h_
#define Protium_PhysicalValue_h_

#include "Protium/Allocation/SmallObject"

namespace Protium{
	namespace PhysicalValues{
		//THIS IS TO BE USED FOR NON COVARIANT VARIABLES
		template<typename T>
		class VariantVariable : public Protium::Allocation::DefaultSmallObject {
			T fValue;
			T fError;
		public:
			VariantVariable(T val, T err) : fValue(val), fError(err) {}
			VariantVariable(const VariantVariable& other) : fValue(other.fValue), fError(other.fError) {}
			VariantVariable& operator=(const VariantVariable& other){
				this->fValue = other.fValue;
				this->fError = other.fError;
				return *this;
			}
			T getValue() const {
				return this->fValue;
			}
			T getError() const {
				return this->fError;
			}
			void setValue(const T& val){
				this->fValue = val;
			}
			void setError(const T& err){
				this->fError = err;
			}
			//Taken From : http://en.wikipedia.org/wiki/Propagation_of_uncertainty
			VariantVariable& operator +=(const VariantVariable& other){
				T err = sqrt(this->fError*this->fError + other.fError*other.fError);
				T val = this->fValue + other.fValue;
				return *this;
			}

			VariantVariable& operator -=(const VariantVariable& other){
				T err = sqrt(this->fError*this->fError - other.fError*other.fError);
				T val = this->fValue + other.fValue;
				this->fError = err;
				this->fValue = val;
				return *this;
			}

			VariantVariable& operator *=(const VariantVariable& other){
				T product = this->fValue*other.fValue;
				T err = abs(product)*sqrt( (this->fError/this->fValue)*(this->fError/this->fValue)+(other.fError/other.fValue)*(other.fError/other.fValue)  );
				this->fValue = product;
				this->fError = err;
				return *this;
			}

			VariantVariable& operator *=(const T& val){
				this->fValue*= val;
				this->fError*=val;
				return *this;
			}

			VariantVariable& operator /=(const VariantVariable& other){
				T product = this->fValue*other.fValue;
				T err = abs(product)*sqrt( (this->fError/this->fValue)*(this->fError/this->fValue)+(other.fError/other.fValue)*(other.fError/other.fValue)  );
				this->fValue = product;
				this->fError = err;
				return *this;
			}

			VariantVariable& operator /=(const T& val){
				this->fValue*= val;
				this->fError*=val;
				return *this;
			}

			const VariantVariable operator+(const VariantVariable& other)const{
				return (VariantVariable(*this)+=other );
			}
			const VariantVariable operator-(const VariantVariable& other)const{
				return (VariantVariable(*this)-=other );	
			}
			const VariantVariable operator*(const VariantVariable& other)const{
				return (VariantVariable(*this)*=other );
			}
			const VariantVariable operator/(const VariantVariable& other)const{
				return (VariantVariable(*this)/=other );
			}
			const VariantVariable operator*(const T& other)const{
				return (VariantVariable(*this)*=other );
			}
			const VariantVariable operator/(const T& other)const{
				return (VariantVariable(*this)/=other);
			}
			VariantVariable pow(const T& other) const{
				T power = pow(this->fValue, other);
				return VariantVariable( power,abs(power*other*this->fError/this->fValue  ) );

			}
			VariantVariable log() const{

				return VariantVariable(log(this->fValue), abs(this->fError/this->fValue));
			};

			VariantVariable logDecimal() const{
				return VariantVariable(log(this->fValue)/log(10.), abs(this->fError/(this->fValue*log(10)) ) );
			}
			VariantVariable exp() const{
				T f = exp(this->fValue);
				return VariantVariable(f,f*f*this->fError*this->fError);
			}
			VariantVariable exp(const T& base)const{
				T f = exp(base, this->fValue);
				return VariantVariable(f, abs(f*log(base)*this->fError ) );
			}

			VariantVariable pow(const VariantVariable& other)const{
				T val = pow(this->fValue, other.fValue);
				T err = abs(f)*sqrt( (other.fValue*this->fError/this->fValue)*(other.fValue*this->fError/this->fValue) +(log(this->fValue)*other.fError )*(log(this->fValue)*other.fError ) );
				return VariantVariable(val, err);
			}
		};


		//TODO: Add in covariant values for propagation.

	}
}

#endif //Protium_PhysicalValue_h_