/**	\file deuterium_port_sniffer
**/
#include "Deuterium/Networking/RawSocket.h"

#include <iostream>

#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>



void PrintIPHeader( struct iphdr* iph){
    std::cout<<"\n";
    std::cout<<"IP Header"<<std::endl;
    std::cout<<"   |-IP Version        : "<<(unsigned int)iph->version<<std::endl;
    std::cout<<"   |-IP Header Length  : "<<(unsigned int)iph->ihl<<" DWORDS or "<<((unsigned int)(iph->ihl))*4<<" Bytes\n"<<std::endl;
    std::cout<<"   |-Type Of Service   : "<<(unsigned int)iph->tos<<std::endl;
    std::cout<<"   |-IP Total Length   : "<<ntohs(iph->tot_len)<<"  Bytes(Size of Packet)\n"<<std::endl;
    std::cout<<"   |-Identification    : "<<ntohs(iph->id)<<std::endl;
    //std::cout<<"   |-Reserved ZERO Field   : %d\n",(unsigned int)iphdr->ip_reserved_zero)<<std::endl;
    //std::cout<<"   |-Dont Fragment Field   : %d\n",(unsigned int)iphdr->ip_dont_fragment)<<std::endl;
    //std::cout<<"   |-More Fragment Field   : %d\n",(unsigned int)iphdr->ip_more_fragment)<<std::endl;
    std::cout<<"   |-TTL      : "<<(unsigned int)iph->ttl<<std::endl;
    std::cout<<"   |-Protocol : "<<(unsigned int)iph->protocol<<std::endl;
    std::cout<<"   |-Checksum : "<<ntohs(iph->check)<<std::endl;
    //get out the source and destination info
    sockaddr_in addr;
    addr.sin_addr.s_addr = iph->saddr;
    std::cout<<"   |-Source IP        : "<<inet_ntoa(addr.sin_addr)<<std::endl;
	addr.sin_addr.s_addr = iph->daddr;
    std::cout<<"   |-Destination IP   : "<<inet_ntoa(addr.sin_addr)<<std::endl;
}


int main(int argc, char* argv[]){
	if(argc<2){
		std::cout<<"Usage: deuterium_port_sniffer <local_port_number>"<<std::endl;
		return -1;
	}

	//I don't really know why I chose this port number
	unsigned short local_port_number = 1080;

	Deuterium::Networking::RawSocketImpl rawSocket;
	//rawSocket.GetLocalInformation();

	//rawSocket.RebindToLocalPortAddress(local_port_number,"localhost");
	//rawSocket.SetupKernel();

	std::cout<<"Setting Up Listener Socket at: "<<rawSocket.GetLocalAddress()<<" : "<<rawSocket.GetLocalPort()<<std::endl;

	//Now that it's setup, let's take a look at 

	while(1){
		rawSocket.GetNext();
		PrintIPHeader(rawSocket.GetCurrentIPHeader() );
		/*
		switch(rawSocket.GetCurrentIPHeader()->iph_protocol ){
			case 0x01:
				PrintICMPHeader(rawSocket.GetICMPHeader() );
				break;
			case 0x06:
				PrintTCPHeader(rawSocket.GetTCPHeader() );
				break;
			case 0x11:
				PrintUDPHeader(rawSocket.GetUDPHeader() );
				break;
			default:
				PrintData(rawSocket.GetData() );
				break;
		};
		*/
	}
	return 0;
}