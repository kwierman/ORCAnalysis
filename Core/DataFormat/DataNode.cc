#include "Deuterium/DataFormat/DataNode.hh"


namespace Deuterium{
	namespace DataFormat{
		DataNode::DataNode(const std::string& name ) : fName(name), fParent(NULL){

		}
		DataNode::~DataNode() {

		}

		DataNode::DataNode(const DataNode& other){
			this->m_children = other.m_children;
			this->fName = other.fName;
			this->fParent = other.fParent;
		}
					//! Copy Assign operator
		DataNode& DataNode::operator=(const DataNode& other){
			this->m_children = other.m_children;
			this->fName = other.fName;
			this->fParent = other.fParent;	
		}

		std::string DataNode::GetName() const{
			return fName;
		}

		std::string DataNode::GetPath() const{
			std::string path;
			if(fParent)
				path = fParent->GetPath();
			path+="/"+this->GetName();
			return path;
		}

		void DataNode::SetName(const std::string& name ){
			fName = name;
		}

		bool DataNode::IsName(const std::string& name) const{
			return name==fName;
		}

		DataNode* DataNode::AddNodeToChildren(const DataNode& node){
			node.fParent = this;
			this->m_children.push_back(node);
			//return back a pointer to the last child
			return &(*m_children.rbegin());
		}

		DataNode::NodeIterator DataNode::GetChildByName(const std::string& name){
			for(DataNode::NodeIterator it = this->m_children.begin(); it!= this->m_children.end(); ++it){
				if( it->IsName(name) ){
					return it;
				}
				else{
					it->GetChildByName(name);
				}
			}
			return m_children.end();
		}

		bool DataNode::RemoveChildByName(const std::string& name){
			for(DataNode::NodeIterator it = m_children.begin(); it!= m_children.end(); ++it){
				if( it->IsName(name) ){
					m_children.erase(it);
					it = m_children.begin();
					return true;
				}
			}
			return false;
		}
	}

}