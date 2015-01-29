#ifndef Deuterium_Socket_h_
#define Deuterium_Socket_h_
#include "Deuterium/Networking/NetworkingException.hh"
#include "Deuterium/Networking/NetworkUtilities.hh"


#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in
#include <errno.h>           // For errno
#include <stdlib.h>
#include <string.h>


//TODO: Remove for debugging
#include <iostream>


namespace Deuterium{
  namespace Networking{

    //!Null Socket Type
    struct InValidSocket{};

    struct RawSocket{
      enum{type=SOCK_RAW, protocol=IPPROTO_TCP}; 
    };
    struct TCPSocket{
      enum{type=SOCK_STREAM, protocol=IPPROTO_TCP};
    };

    struct UDPSocket{
      enum{type=SOCK_DGRAM, protocol=IPPROTO_UDP};
    };

    //! Prototype class for all socket types. Defaults to a TCP socket due to ease of TCP.
    template<class SocketType=TCPSocket>
    class SocketPrototype : public SocketType {
    public:
      //! Holds the socket descriptor
      int fSockDesc;
      //! Holds the local address and port
      std::string fLocalAddress;
      //! Holds local port number
      unsigned short fLocalPort;
      //! Validity flag
      bool fIsPortValid;

      //!Default constructor uses template to create socket type,protocol
      SocketPrototype(): fSockDesc(-1) {
        fIsPortValid=false;
        fSockDesc =socket(PF_INET, SocketType::type, SocketType::protocol);
        fIsPortValid = (fSockDesc>=0);
        SocketPrototype::GetLocalInformation();
      }
      SocketPrototype(const int& socketDesc){
        fSockDesc = socketDesc;
        GetLocalInformation();
      }

      //!  Closes out the socket
      virtual ~SocketPrototype(){
        if(fSockDesc>0) 
          close(fSockDesc);
        fSockDesc=-1;
        fIsPortValid=false;
      }

      //! Needs to be called whenever local information is created or updated in order to fetch back info
      void GetLocalInformation(){
        sockaddr_in addr;
        unsigned int addr_len = sizeof(addr);

        if (getsockname(fSockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0)
          throw NetworkingException("Fetch of local information failed (getsockname())", true);
        fLocalAddress=inet_ntoa(addr.sin_addr);
        fLocalPort=ntohs(addr.sin_port);
      }
      //! Gets local Address
      std::string GetLocalAddress(){return fLocalAddress;}
      //! Gets local port number
      unsigned short GetLocalPort(){return fLocalPort;}
      //! Rebinds this socket to a port and address
      //! \note defaults to 0, and the local address
      void RebindToLocalPortAddress(const unsigned short& port=0, const std::string& address="" ){
        sockaddr_in localAddr;
        if(address=="")
          Deuterium::Networking::fillAddr(this->GetLocalAddress(), port, localAddr);
        else
          Deuterium::Networking::fillAddr(address, port, localAddr);
        if(fSockDesc>0)
          close(fSockDesc);
        fSockDesc=-1;
        fSockDesc =socket(PF_INET, SocketType::type, SocketType::protocol);
        if (bind(fSockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0)
          throw NetworkingException("Set of local port failed (bind())", true);
        GetLocalInformation();  
      }
      //Resolves a service number by ... TODO:: Fix it, Mack.
      /*
      unsigned short ResolveService(const std::string& service,const std::string& protocol);
      */
      
    };


    class TransmitterSocket : public SocketPrototype<TCPSocket>{
    public:
      std::string fRemoteAddress;
      unsigned short fRemotePort;

      TransmitterSocket() : SocketPrototype(){}
      TransmitterSocket(const int& socketDescriptor) : SocketPrototype(socketDescriptor){}

      bool Connect(const unsigned short& remotePort, const std::string& remoteAddress ) {//throw(NetworkingException){
        sockaddr_in destAddr;
        fillAddr(remoteAddress, remotePort, destAddr);

        int something = connect(fSockDesc, (sockaddr *) &destAddr, sizeof(destAddr));

        return ( something >= 0) ;
          //throw NetworkingException("Connect failed (connect())", true);
      }
      void GetRemoteInfo(){
        sockaddr_in addr;
        unsigned short addr_len = sizeof(addr);        
        if (getpeername(fSockDesc, (sockaddr *) &addr,(socklen_t *) &addr_len) < 0)
          throw NetworkingException("Fetch of foreign address failed (getpeername())", true);
        fRemoteAddress=inet_ntoa(addr.sin_addr);
        fRemotePort=ntohs(addr.sin_port);
      }

      std::string GetRemoteAddress() const {return fRemoteAddress;}
      unsigned short GetRemotePort()  const {return fRemotePort;}

      bool Send(const void* buffer, int bufferLen){
        return (send(fSockDesc, buffer, bufferLen, 0) < 0);
      }
      int Receive(void* buffr, int bufferLen) throw(NetworkingException){
        return recv(fSockDesc, buffr, bufferLen,0);
      }



    };

    class ListenerSocket : public SocketPrototype<TCPSocket>{
    public:
      bool Listen(unsigned queueLength){
        return (listen(fSockDesc, queueLength) < 0);
      }
      TransmitterSocket* Accept(){
        int newConnSD;
        if ((newConnSD = accept(fSockDesc, NULL, 0)) < 0)
          throw NetworkingException("Accept failed (accept())", true);
          return new TransmitterSocket(newConnSD);
      }

    };

    class BroadcastSocket : public SocketPrototype<UDPSocket>{
      BroadcastSocket(const unsigned short& localPort=0, const std::string& localAddress=""){
        RebindToLocalPortAddress(localPort, localAddress);
      }

      void BroadCast(){
        int broadcastPermission = 1;
        setsockopt(fSockDesc, SOL_SOCKET, SO_BROADCAST, 
             (void *) &broadcastPermission, sizeof(broadcastPermission));
      }
      void Disconnect(){
        sockaddr_in nullAddr;
        memset(&nullAddr, 0, sizeof(nullAddr));
        nullAddr.sin_family = AF_UNSPEC;
        // Try to disconnect
        if (::connect(fSockDesc, (sockaddr *) &nullAddr, sizeof(nullAddr)) < 0) {
          if (errno != EAFNOSUPPORT) {
            throw NetworkingException("Disconnect failed (connect())", true);
          }
        }
      }
      bool SendTo(const void* buffer, int bufferLen, const std::string& remoteAddress, const unsigned short& remotePort){
        sockaddr_in remoteAddr;
        fillAddr(remoteAddress.c_str(), remotePort, remoteAddr);
        return (sendto(fSockDesc, buffer, bufferLen, 0, (sockaddr*)&remoteAddr, sizeof(remoteAddr))!=bufferLen );

      }
      int ReceiveFrom(void* buffer, int bufferLen, std::string& remoteAddress, unsigned short& remotePort){
        sockaddr_in clntAddr;
        socklen_t addrLen = sizeof(clntAddr);      
        int rtn =recvfrom(fSockDesc, buffer, bufferLen, 0,(sockaddr*) &clntAddr, (socklen_t*) &addrLen); 
        remoteAddress = inet_ntoa(clntAddr.sin_addr);
        remotePort = ntohs(clntAddr.sin_port);
      }
      bool SetMulticastTTL(unsigned char multicastTTL){
        return (setsockopt(fSockDesc, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &multicastTTL, sizeof(multicastTTL))<0);
      }
      bool JoinGroup(const std::string& multicastGroup){
        struct ip_mreq multicastRequest;      
        multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.c_str());
        multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
        return (setsockopt(fSockDesc, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
                 (void*) &multicastRequest, 
                 sizeof(multicastRequest)) < 0);
      }
      bool LeaveGroup(const std::string& multicastGroup){
        struct ip_mreq multicastRequest;

        multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup.c_str());
        multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
        return (setsockopt(fSockDesc, IPPROTO_IP, IP_DROP_MEMBERSHIP, 
                 (void*) &multicastRequest, 
                 sizeof(multicastRequest)) < 0);
      }
    };
  }
}

#endif //File Guardian

