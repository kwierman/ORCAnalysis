#include "Deuterium/DataFormat/StringParser.hh"
#include "Deuterium/Networking/Socket.hh"

#include <iostream>
#include <vector>

using Deuterium::DataFormat::StringParser;
using Deuterium::Networking::TransmitterSocket;
//using Deuterium::Networking::Socket;

//using Deuterium::Networking::Client;


int main(int argc, char* argv[]){

	TransmitterSocket socket;
	//std::cout<<"SocketInfo: "<<socket.GetLocalAddress()<<" : "<<socket.GetLocalPort()<<std::endl;
	//socket.RebindToLocalPortAddress(1080,"127.0.0.1");
	//std::cout<<"SocketInfo: "<<socket.GetLocalAddress()<<" : "<<socket.GetLocalPort()<<std::endl;
	//socket.RebindToLocalPortAddress(1100,"0.0.0.1");
	//std::cout<<"SocketInfo: "<<socket.GetLocalAddress()<<" : "<<socket.GetLocalPort()<<std::endl;

	//Enter in the remote address
	std::cout<<"Local Server Info: "<<socket.GetLocalAddress()<<" : "<<socket.GetLocalPort()<<std::endl;

	std::cout<<"Please Enter Remote Server Address: "<<std::endl;
	std::string address;
	std::cin >> address;

	//Enter in the port(s) which wish to use

	std::cout<<"Please enter a list of ports you wish to use."<<std::endl;
	std::cout<<"\tDelimit numbers with ',' and ranges with '-'"<<std::endl;

	std::string port_list;
	std::cin >> port_list;

	//socket.SetLocalPort(120);//why not

	StringParser<unsigned> parser(port_list);
	std::vector<unsigned> ports = parser.delimit(',').makerange('-').AsVector();

	
	for(std::vector<unsigned>::iterator it = ports.begin(); it!= ports.end(); ++it){
		if(socket.Connect( (*it), address ) )
			std::cout<<"Port: "<<(*it)<<" Open"<<std::endl;
	}
	
}
