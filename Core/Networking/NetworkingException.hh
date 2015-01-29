#ifndef Deuterium_NetworkingException_hh_
#define Deuterium_NetworkingException_hh_

#include <exception>
#include <string>            // For string

namespace Deuterium{

	namespace Networking{

		class NetworkingException : public std::exception {
		public:
			NetworkingException(const std::string &message, bool inclSysMsg = false) throw();
			~NetworkingException() throw();	
			const char *what() const throw();
		private:
			std::string userMessage;  // Exception message
		};
	}
}

#endif //File Guardian