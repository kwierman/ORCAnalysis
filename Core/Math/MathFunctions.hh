#ifndef Protium_MathFunction_hh_
#define Protium_MathFunction_hh_

namespace Protium{

	namespace Math{

		template<typename T>
		T factorial(T x){
			T y=T(1.0);
			while(x>0)
				y*=(x--);
			return y;
		}

		long double one_over_factorial(unsigned long i);
		long double compute_e();

		//A couple of important numbers  
		static const long double euler = 2.71828182845904523536028747135266249775724709369995;
		static const double nat = compute_e();//2.71828182846;
		static const double dbl_epsilon = 2.2204460492503131e-15;
		static double dbl_omega =  1.7e308;

		double powerf3(double& x, double& y, unsigned iter);
		double powerf2(double& x, double& y);

		double power(double x, double y);

		double exponent(double x);

		double exponent_taylor(double x, int n_iter);

		double log_taylor(double x, int n_iter);

		double absolute(const double x);

		template<typename T>
		unsigned int n_digits(T x);

		template<typename T>
		int ith_digit(T x, unsigned  i);

		double Gamma(double x);

		double babylonian_sqrt(double x, int n_iter=100);

		double digit_sqrt(double x, int n_digits);

		double sqrt_from_exp(double x, int n_iter);

		//only valid for 0<x<3. Best for x approx 1
		double sqrt_2_var(double x);
	}
}


#endif  //File Guardian