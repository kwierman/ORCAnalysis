
#include <iostream>
#include <string>

#include "Deuterium/DataFormat/BaseFormat.h"
#include "Deuterium/DataFormat/JSONFormat.h"
#include "Deuterium/DataFormat/XMLFormat.h"

using Deuterium::DataFormat::DataNode;
using Deuterium::DataFormat::JSONFormat;
using Deuterium::DataFormat::XMLFormat;

DataNode MakeArray(const std::string& name){
	DataNode namedArray(DataNode::pair_type);
	namedArray.SetString(name);

	namedArray.Append( DataNode(DataNode::array_type) );

	return namedArray;
}


int main(int argc, char* argv[]){

	//Ensure proper usage
	if(argc<2){
		std::cout<<"Usage: deuterium_input_test <filename>"<<std::endl;
		return -1;
	}

	DataNode data;

	for(int i=0; i<4;i++){
		DataNode stringNode(DataNode::string_type);
		stringNode.SetString("This is a node");

		data.Append(stringNode);
	}
	for(int i=0; i<4;i++){
		DataNode intNode(DataNode::int_type);
		intNode.SetInt(i);

		data.Append(intNode);
	}
	for(int i=0; i<4;i++){
		data.Append( MakeArray("This is an array") );
	}


	std::cout<<sizeof(Deuterium::DataFormat::DataNode)<<std::endl;

	JSONFormat json;
	json.SetNode(data);

	std::cout<<json.Write()<<std::endl;

	XMLFormat xml;
	xml.SetNode(data);

	std::cout<<xml.Write()<<std::endl;

	/*
	//Make sure the file exists
	Deuterium::FileSystem::File input(argv[1]);
	if(! input.IsValid() ){
		std::cout<<"Invalid filename"<<std::endl;
		return -1;
	}

	//Create 
	Deuterium::DataFormat::Parser<Deuterium::DataFormat::DOMParser> parser(input);
	parser.Read();

	if(!parser.IsGood() ){
		std::cout<<"Parser Encountered Error"<<std::endl;
		return -1;
	}

	Deuterium::DataFormat::Parser<Deuterium::DataFormat::DOMParser>::Iterator it=NULL;

	while(parser.HasNext() ){
		it = parser.GetNext();
		PrintDataNode(it);
	}
	*/
}