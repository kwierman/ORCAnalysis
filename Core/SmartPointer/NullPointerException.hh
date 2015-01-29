#ifndef ORCA_NullPointerException_hh_
#define ORCA_NullPointerException_hh_

#include <stdexcept>

namespace ORCA{
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