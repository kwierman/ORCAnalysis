#ifndef Deuterium_NamedException_h_
#define Deuterium_NamedException_h_

#include <exception>
#include <string>            

namespace Deuterium{

	namespace Exception{

		class NamedException : public std::exception {
		public:
			NamedException(const std::string &message, bool inclSysMsg = false) throw();
			~NamedException() throw();	
			const char *what() const throw();
		private:
			std::string userMessage;  // Exception message
		};
	}
}

#endif //File Guardian