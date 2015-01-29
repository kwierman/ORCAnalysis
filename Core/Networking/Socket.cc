#include "Deuterium/Networking/Socket.hh"

#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in
#include <errno.h>           // For errno


//Resolves a service number by 
/*
unsigned short Deuterium::Networking::SocketPrototype::ResolveService(const std::string& service,const std::string& protocol) {
struct servent *serv; 
if ((serv = getservbyname(service.c_str(), protocol.c_str())) == NULL)
  return atoi(service.c_str());
else 
  return ntohs(serv->s_port);  
}
*/
