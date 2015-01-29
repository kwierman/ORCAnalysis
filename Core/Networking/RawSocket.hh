#ifndef Deuterium_RawSocket_hh_
#define Deuterium_RawSocket_hh_

#include "Deuterium/Networking/Socket.hh"
#include "Deuterium/Networking/NetworkingException.hh"

#include <vector>
#include <iostream>
#include <string>

#include <netinet/ip_icmp.h>//for icmp header
#include <netinet/ip.h>//for ip header
#include <netinet/udp.h>//for udp header
#include <netinet/tcp.h>//for tcp header

#include <unistd.h>//for getting super user


#define PCKT_LEN 8192


/* Common Protocol Numbers:
	1 	Internet Control Message Protocol 	ICMP
	2 	Internet Group Management Protocol 	IGMP
	6 	Transmission Control Protocol 	TCP
	17 	User Datagram Protocol 	UDP
	41 	IPv6 encapsulation 	ENCAP
	89 	Open Shortest Path First 	OSPF
	132 	Stream Control Transmission Protocol 	SCTP
*/

namespace Deuterium{
	namespace Networking{

		struct ipv4header {
			unsigned char      iph_ihl:5,// Internet Header Length, the number of 32-bit words in the header (count them, there's 5 in IPV4), unless options specify otherwise
							   iph_ver:4;//ip version, usually 4
			unsigned char      iph_tos;  //Type-Of-Service (deprecated), Now DSCP and ECN, Think of it as a 0-16 Number that represent prority
			unsigned short int iph_len; //Total-Length. Length of this, and any other headers wrapping content
			unsigned short int iph_ident;//Use to identify packets together, use with htons
			unsigned char      iph_flag;//Fragmentation flags, 2 for Don't fragment (may result in packet drop). 4 for many fragments.
			unsigned short int iph_offset;//Specifies the particular fragment offset from the original packet. Set to zero for no fragmented
			unsigned char      iph_ttl;//Time to Live. Set to 64, usually
			unsigned char      iph_protocol;// Check http://en.wikipedia.org/wiki/List_of_IP_protocol_numbers for list of IP numbers
			unsigned short int iph_chksum;// Header checksum. Kernel needs to take care of this
			unsigned int       iph_sourceip;// Set the Source IP here using inet_addr()
 			unsigned int       iph_destip;// Destination IP address
		};

		struct tcpheader {
			unsigned short int tcph_srcport;//Which port to communicate at the source
			unsigned short int tcph_destport;//Which port to communicate with at the destination
			unsigned int       tcph_seqnum;//Set to htonl(1)
			unsigned int       tcph_acknum;//Set to 0
			unsigned char      tcph_reserved:4, 
								tcph_offset:4;//Set to 5
			unsigned int
				tcp_res1:4,       /*little-endian*/
				tcph_hlen:4,      /*length of tcp header in 32-bit words*/
				tcph_fin:1,       /*Finish flag "fin"*/
				tcph_syn:1,       /*Synchronize sequence numbers to start a connection*/
				tcph_rst:1,       /*Reset flag */
				tcph_psh:1,       /*Push, sends data to the application*/
				tcph_ack:1,       /*acknowledge*/
				tcph_urg:1,       /*urgent pointer*/
				tcph_res2:2;		/*reserved*/
			unsigned short int tcph_win;
			unsigned short int tcph_chksum;
			unsigned short int tcph_urgptr;
		};

		struct udpheader {
			unsigned short int udph_srcport;
			unsigned short int udph_destport;
			unsigned short int udph_len;
			unsigned short int udph_chksum;
		};

/* ICMP Types
	Type 0 — Echo Reply
	Type 3 — Destination Unreachable
	Type 4 — Source Quench (Deprecated)
	Type 5 — Redirect
	Type 6 — Alternate Host Address (Deprecated)
	Type 7 — Unassigned
	Type 8 — Echo
	Type 9 — Router Advertisement
	Type 10 — Router Selection
	Type 11 — Time Exceeded
	Type 12 — Parameter Problem
	Type 13 — Timestamp
	Type 14 — Timestamp Reply
	Type 15 — Information Request (Deprecated)
	Type 16 — Information Reply (Deprecated)
	Type 17 — Address Mask Request (Deprecated)
	Type 18 — Address Mask Reply (Deprecated)
	Type 19 — Reserved (for Security)
Types 20-29 — Reserved (for Robustness Experiment)
Type 30 — Traceroute (Deprecated)
Type 31 — Datagram Conversion Error (Deprecated)
Type 32 — Mobile Host Redirect (Deprecated)
Type 33 — IPv6 Where-Are-You (Deprecated)
Type 34 — IPv6 I-Am-Here (Deprecated)
Type 35 — Mobile Registration Request (Deprecated)
Type 36 — Mobile Registration Reply (Deprecated)
Types 37 — Domain Name Request (Deprecated)
Types 38 — Domain Name Reply (Deprecated)
Type 39 — SKIP (Deprecated)
Type 40 — Photuris
Type 41 — ICMP messages utilized by experimental mobility protocols such as Seamoby
Types 42-252 — Unassigned
Type 253 — RFC3692-style Experiment 1 
*/

		typedef struct _icmphdr{
		    unsigned char   i_type;
		    unsigned char   i_code;                 // Type sub code
		    unsigned short i_cksum;
		    unsigned short i_id;
		    unsigned short i_seq;
		    // This is not the standard header, but we reserve space for time
		    unsigned long  timestamp;
		} IcmpHeader;

		typedef struct _ipoptionhdr{
		    unsigned char        code;        // Option type
		    unsigned char        len;         // Length of option hdr
		    unsigned char        ptr;         // Offset into options
		    unsigned long        addr[9];     // List of IP addrs
		} IpOptionHeader;

/*ICMP Options:
		icmp->type = ICMP_ECHO;
        icmp->code = 0;
        icmp->checksum = htons(~(ICMP_ECHO << 8));

*/

		//! Raw Socket allows us to implement our own version of the header scheme for connections
		//! \warning RAW SOCKETS CAN ONLY BE USED BY ROOT USER
		class RawSocketImpl : public SocketPrototype<RawSocket>{
		public:
			//! Data for the sender info
			sockaddr_in fLocalInfo;
			//! Data for the receiver info
			sockaddr_in fRemoteInfo;
			//! Buffer Data
			char* fBuffer;


			RawSocketImpl() : SocketPrototype()  {//: fBuffer(65536)
				//TODO: Change this up to 
				//fSockDesc=socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
				//fLocalInfo.sin_family = AF_INET;
				//fRemoteInfo.sin_family = AF_INET;
				fBuffer = new  char[65536];

				std::cout<<"In raw socket implementation default constructor"<<std::endl;


				if(getuid() && geteuid() ){
					std::cout<<"Not Super User. Will not be able to open up raw sockets"<<std::endl;
				}

			}

			~RawSocketImpl(){
				delete fBuffer;
			}

			void SetLocalPortAddress(const unsigned short& port, const std::string& address){
				fLocalInfo.sin_addr.s_addr = htons(port);
				//Setup the local address
			}

			void SetRemotePortAddress(const unsigned short& port, const std::string& address){
				fRemoteInfo.sin_addr.s_addr = htons(port);
				//Setup the remote address
			}

			void SetupKernel(){
				//inform the kernel that we're using our own headers
				int one=1;
				const int* val=&one; 
				setsockopt(fSockDesc, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));
			}

			void SendTo(void* buffer, unsigned bufferLen){
				//sendto(fLocalInfo, buffer, ip->iph_len, 0, (struct sockaddr *)&sin, sizeof(sin))
			}

			void GetNext(){
				//his should be implemented to
				std::cout<<"Socket Desc: "<<fSockDesc<<std::endl;
		        sockaddr_in addr;
		        unsigned int addr_len = sizeof(addr);
		        if (getsockname(fSockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0)
		          throw NetworkingException("Get Next of local information failed (getsockname())", true);
		        fLocalAddress=inet_ntoa(addr.sin_addr);
		        fLocalPort=ntohs(addr.sin_port);
		        std::cout<<"Waiting For Data: "<<std::endl;
				int datasize = recvfrom(fSockDesc , &fBuffer[0] , 65536*sizeof(char) , 0 , (sockaddr*)&fRemoteInfo , &addr_len);
				std::cout<<"DataSize: "<<datasize<<std::endl;
			}

			struct iphdr* GetCurrentIPHeader(){
				return (iphdr*)&fBuffer[0];
			}	

			std::string GetRemoteAddress(){
				return std::string(inet_ntoa(fRemoteInfo.sin_addr));
			}		
			unsigned short GetRemotePort(){
				return ntohs(fRemoteInfo.sin_port);
			}		

		};
	}
}

#endif //File guardian