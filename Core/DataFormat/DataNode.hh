#ifndef ORCA_DataNode_hh_
#define ORCA_DataNode_hh_

#include "ORCA/Allocation/SmallObject.hh"
#include "ORCA/Allocation/STLAdapter.hh"

#include <string>
#include <vector>


namespace ORCA{
	namespace DataFormat{

		/*! \class DataNode 
			\brief Abstract for node tree structure
		*/
		class DataNode : public ORCA::Allocation::DefaultSmallObject {
			//! Child Nodes
			std::string fName;
			std::vector<DataNode, ORCA::Allocation::STLAdapter<DataNode> > m_children;
			const mutable DataNode* fParent;

		public:
			typedef std::vector<DataNode, ORCA::Allocation::STLAdapter<DataNode> >::iterator NodeIterator;
			DataNode(const std::string& name="" );
			//DataNode(const char* name = "") : fName(name){}
			virtual ~DataNode();
			//! Copy Constructor
			DataNode(const DataNode& other);
			//! Copy Assign operator
			DataNode& operator=(const DataNode& other);

			DataNode* GetParent(){
				return const_cast<DataNode*>(fParent);
			}

			unsigned GetNChildren(){
				return m_children.size();
			}

			std::string GetName() const;
			std::string GetPath() const;

			void SetName(const std::string& name );

			bool IsName(const std::string& name) const;

			DataNode* AddNodeToChildren(const DataNode& node);

			NodeIterator GetChildByName(const std::string& name);
			NodeIterator begin(){return m_children.begin();}
			NodeIterator end(){return m_children.end();}
			bool IsEnd(const NodeIterator& it){
				return (it == m_children.end());
			}

			bool RemoveChildByName(const std::string& name);

		};
	}
}



#endif //File Guardian


