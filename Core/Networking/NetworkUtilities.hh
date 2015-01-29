#ifndef ORCA_NetworkUtilities_h_
#define ORCA_NetworkUtilities_h_

#include <string>
#include <netinet/in.h>      // For sockaddr_in

/**
	\file NetworkUtilities.hh
	\brief Some utilities to help with networking
**/


namespace ORCA{
	namespace Networking{
		static void fillAddr(const std::string &address, unsigned short port, sockaddr_in &addr);
	    static  unsigned short csum(unsigned short *buf, int len);
  }
}



#endif //File Guardian