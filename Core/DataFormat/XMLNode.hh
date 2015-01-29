#ifndef Deuterium_XMLNode_hh_
#define Deuterium_XMLNode_hh_

#include "Deuterium/DataFormat/DataNode.hh"

namespace Deuterium{

	namespace DataFormat{

		//! Represents attributes applicable to XML elements (i.e. name="value" )
		//! Shows up in the tree as : attribute : {{name, "name"} ,{value, "value"} }
		struct XMLAttributeNode : DataNode{
			XMLAttributeNode(const std::string& name, const std::string& value);
			std::string AttributeName();
			std::string AttributeValue();
			void SetName(const std::string& name);
			void SetValue(const std::string& value);
		};

		//! Represents the comments inserted into code
		//! Shows up as <comment : {value}>
		struct XMLCommentNode :  DataNode {
			XMLCommentNode(const std::string& text);
			void SetValue(const std::string& value);
			std::string GetValue();
		};

		//! Represents the data held within elements
		//! Shows up as: data : {""}
		struct XMLDataNode :  DataNode {
			XMLDataNode(const std::string& text);
			void SetValue(const std::string& value);
			std::string GetValue();
		};


		//! Implements all the functionality for xml objects
		struct XMLElementNode :  DataNode {
			XMLElementNode(const std::string& name);
			
			void AddAttribute(const std::string& name, const std::string& value);
			void AddComment(const std::string& text);
			void AddData(const std::string& text);

			XMLAttributeNode* GetAttribute(const std::string& name);
			std::string GetAttributeValue(const std::string& name);

			NodeIterator GetFirstAttribute();
			NodeIterator GetNextAttribute(const NodeIterator& input);

			NodeIterator GetFirstComment();
			NodeIterator GetNextComment(const NodeIterator& input);
			
			NodeIterator GetFirstData();
			NodeIterator GetNextData(const NodeIterator& input);
		};

		struct XMLDTDNode : DataNode {
			XMLDTDNode(const std::string& version, const std::string& encoding);
			std::string GetVersion();
			std::string GetEncoding();
			void SetVersion(const std::string& version);
			void SetEncoding(const std::string& encoding);
		};
	}
}



#endif //File Guardian
