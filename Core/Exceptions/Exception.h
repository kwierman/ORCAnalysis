#ifndef Deuterium_Excpetion_h_
#define Deuterium_Excpetion_h_

#include <exception>

namespace Deuterium{

	namespace Exception{

		class Exception : public std::exception{

		public:
			Exception() throw();
			Exception(const Exception& other) throw();
			Exception& operator=(const Excetion& other) throw();

			virtual ~Exception() throw();
			virtual const char* what(){return "Deuterium Error";}
		};

		Exception::Exception() : std::exception() {}
		Exception::Exception(const Exception& other) : std::exception(other) {this = &other;}
		Exception& Exception::operator=(const Exception& other){return *this;}
		Exception::~Exception() {}
	}
}

#endif //File Guardian

