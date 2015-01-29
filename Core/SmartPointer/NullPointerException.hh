#ifndef Protium_NullPointerException_hh_
#define Protium_NullPointerException_hh_

#include <stdexcept>

namespace Protium{
	namespace SmartPointer{
	    struct NullPointerException : public std::runtime_error{
	        NullPointerException() : std::runtime_error(std::string(""))
	        { }
	        const char* what() const throw()
	        { return "Null Pointer Exception"; }
	    };		
	}
}

#endif //File Guardian