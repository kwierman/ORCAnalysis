#include "Deuterium/Options/Options.hh"
#include "Deuterium/DataFormat/DataNode.hh"
using Deuterium::DataFormat::DataNode;
//#include "Deuterium/DataFormat/DataReader.hh"
//using Deuterium::DataFormat::DataReader;
#include "Deuterium/DataFormat/XMLNode.hh"
using Deuterium::DataFormat::XMLElementNode;
#include "Deuterium/DataFormat/XMLDocumentBuilder.hh"

#include "Protium/Utilities/SignalCatching.hh"


#include <iostream>
#include <string>

class TestClass  : public DataNode{
	int someData;
	static int counter;
public:
	TestClass() : someData(counter++) {}
	TestClass(const TestClass& other) : someData(other.someData){}
	TestClass(const char* name) : DataNode(name), someData(counter++) {}
};
int TestClass::counter = 0;


void PrintDataNode(DataNode& node, const int& indent){
	for(int i=0; i<indent;i++)
		std::cout<<"\t";
	std::cout<<"\""<<node.GetName()<<"\""<<std::endl;//<<" at: "<<node.GetPath()<<std::endl;
		int newIndent=indent+1;
	for(DataNode::NodeIterator it = node.begin(); !node.IsEnd(it) ;++it ){
		PrintDataNode((*it), newIndent);
	}	
}



int main(int argc, char* argv[]){
	Protium::Utilities::SignalCatcher::Instance().Setup();

	//Deuterium::Options::OptionList opt(argc, argv);
	//opt.parse();

	//Deuterium::Options::Option option = opt.getOption('l');
	{
		TestClass root("root");
		std::cout<<root.GetName()<<std::endl;
		TestClass child1("something");
		std::cout<<child1.GetName()<<std::endl;

		root.AddNodeToChildren(child1);


		std::cout<<root.GetChildByName("something")->GetPath()<<std::endl;
		root.RemoveChildByName("something");
	}
	{
		XMLElementNode root(root);
		root.AddAttribute("name","something");
		std::cout<<root.GetAttributeValue("name")<<std::endl;
	}
	{
		std::cout<<"Parsing last one"<<std::endl;
		std::string name = "<?xml version=\"1.0\" encoding=\"UTF8\"?>\
					<node1>\
						<nestedNode1 attribute1=\"something\"/>\
						<nestedNode11 attribute12=\"somethi2ng\" />\
						<nestedNode2/>\
						<nestedNode3>\
							slkfjhgsdlgjh\
							qQwWeErRtyuiopasdfghhklzxcvbnmm\
							QWERTYUIOPLKJHGFDSAZXVCBNM,.\
						</nestedNode3>\
						<!-- something commentwise-->\
						<nodething>\
							<nodewhat/>\
							<nodething3>\
								<nodethingwhatever/>\
							</nodething3>\
						</nodething>\
					</node1>";

		Deuterium::DataFormat::DOMBuilder proc;
		proc.Init();
		for(int i=0; i<name.length(); ++i){
			proc.Process(name[i]);
		}
		if(proc.IsClosed()){
			std::cout<<"XML Finished properly"<<std::endl;
		}
		else{
			std::cout<<"XML Did not get to the end"<<std::endl;
		}
		PrintDataNode(proc.GetDTDNode(),0);
		PrintDataNode(proc.GetRootNode(),0);

	}

	/*
	reader.Parse("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
					<node1>\
						<nestedNode1/>\
						<nestedNode2/>\
						<nestedNode3>\
							slkfjhgsdlgjh\
						</nestedNode3>\
						<!-- something commentwise-->\
					</node1>");

	*/


}