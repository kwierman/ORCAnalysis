#include "Deuterium/DataFormat/XMLNode.hh"


namespace Deuterium{
	namespace DataFormat{

		XMLAttributeNode::XMLAttributeNode(const std::string& name, const std::string& value) : DataNode("attribute"){
			DataNode name_root("name");
			DataNode name_value(name);
			DataNode value_root("value");
			DataNode value_value(value);
			name_root.AddNodeToChildren(name_value);
			value_root.AddNodeToChildren(value_value);
			this->AddNodeToChildren(name_root);
			this->AddNodeToChildren(value_root);
		}
		std::string XMLAttributeNode::AttributeName(){
			return this->GetChildByName("name")->begin()->GetName();
		}
		std::string XMLAttributeNode::AttributeValue(){
			return this->GetChildByName("value")->begin()->GetName();
		}
		void XMLAttributeNode::SetName(const std::string& name){
			this->GetChildByName("name")->begin()->SetName(name);
		}
		void XMLAttributeNode::SetValue(const std::string& value){
			this->GetChildByName("value")->begin()->SetName(value);
		}

		XMLCommentNode::XMLCommentNode(const std::string& text) : DataNode("comment") {
			DataNode data(text);
			this->AddNodeToChildren(data);
		}
		void XMLCommentNode::SetValue(const std::string& value){
			this->begin()->SetName(value);
		}
		std::string XMLCommentNode::GetValue(){
			return this->begin()->GetName();
		}			

		XMLDataNode::XMLDataNode(const std::string& text) : DataNode("data") {
			DataNode data(text);
			this->AddNodeToChildren(data);
		}
		void XMLDataNode::SetValue(const std::string& value){
			this->begin()->SetName(value);
		}
		std::string XMLDataNode::GetValue(){
			return this->begin()->GetName();
		}				


		XMLElementNode::XMLElementNode(const std::string& name) : DataNode(name) {}
			
		void XMLElementNode::AddAttribute(const std::string& name, const std::string& value){
			XMLAttributeNode attr(name, value);
			this->AddNodeToChildren(attr);
		}
		void XMLElementNode::AddComment(const std::string& text){
			XMLCommentNode node(text);
			this->AddNodeToChildren(node);
		}
		void XMLElementNode::AddData(const std::string& text){
			XMLDataNode node(text);
			this->AddNodeToChildren(node);
		}

		XMLAttributeNode* XMLElementNode::GetAttribute(const std::string& name){
			//iterate across the attributes
			for(DataNode::NodeIterator it = this->begin(); it!= this->end(); ++it){
				if( it->GetName() == "attribute" ){
					XMLAttributeNode* attr =  static_cast<XMLAttributeNode*>( &(*it) );
					if(attr){
						if(attr->AttributeName() == name ){
							return attr;
						}
					}
				}
			}
			return 0;
		}
		std::string XMLElementNode::GetAttributeValue(const std::string& name){
			XMLAttributeNode* node = GetAttribute(name);
			if(!node)
				return std::string("");
			return node->AttributeValue();
		}

		DataNode::NodeIterator XMLElementNode::GetFirstAttribute(){
			//iterate across the attributes
			for(DataNode::NodeIterator it = this->begin(); it!= this->end(); ++it){
				if( it->GetName() == "attribute" ){
					return it;
				}
			}
			return this->end();
		}
		DataNode::NodeIterator XMLElementNode::GetNextAttribute(const DataNode::NodeIterator& input){
			//iterate across the attributes
			NodeIterator begin = input;
			for(DataNode::NodeIterator it = ++begin ; it!= this->end(); ++it){
				if( it->GetName() == "attribute" ){
					return it;
				}
			}
			return this->end();				
		}
		DataNode::NodeIterator XMLElementNode::GetFirstComment(){
			//iterate across the Comments
			for(DataNode::NodeIterator it = this->begin(); it!= this->end(); ++it){
				if( it->GetName() == "comment" ){
					return it;
				}
			}
			return this->end();
		}
		DataNode::NodeIterator XMLElementNode::GetNextComment(const DataNode::NodeIterator& input){
			//iterate across the Comments
			NodeIterator begin = input;
			for(DataNode::NodeIterator it = ++begin ; it!= this->end(); ++it){
				if( it->GetName() == "comment" ){
					return it;
				}
			}
			return this->end();				
		}

		DataNode::NodeIterator XMLElementNode::GetFirstData(){
			//iterate across the Datas
			for(DataNode::NodeIterator it = this->begin(); it!= this->end(); ++it){
				if( it->GetName() == "data" ){
					return it;
				}
			}
			return this->end();
		}
		DataNode::NodeIterator XMLElementNode::GetNextData(const DataNode::NodeIterator& input){
			//iterate across the Datas
			NodeIterator begin = input;
			for(DataNode::NodeIterator it = ++begin ; it!= this->end(); ++it){
				if( it->GetName() == "data" ){
					return it;
				}
			}
			return this->end();				
		}
		XMLDTDNode::XMLDTDNode(const std::string& version, const std::string& encoding) : DataNode("dtd"){
			DataNode name_root("version");
			DataNode name_value(version);
			DataNode value_root("encoding");
			DataNode value_value(encoding);
			name_root.AddNodeToChildren(name_value);
			value_root.AddNodeToChildren(value_value);
			this->AddNodeToChildren(name_root);
			this->AddNodeToChildren(value_root);				
		}
		std::string XMLDTDNode::GetVersion(){
			return this->GetChildByName("version")->begin()->GetName();
		}
		std::string XMLDTDNode::GetEncoding(){
			return this->GetChildByName("encoding")->begin()->GetName();
		}
		void  XMLDTDNode::SetVersion(const std::string& version){
			this->GetChildByName("version")->begin()->SetName(version);
		}
		void  XMLDTDNode::SetEncoding(const std::string& encoding){
			this->GetChildByName("encoding")->begin()->SetName(encoding);
		}
	}
}

