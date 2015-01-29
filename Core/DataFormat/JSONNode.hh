#ifndef ORCA_JSONNode_hh_
#define ORCA_JSONNode_hh_

#include "ORCA/DataFormat/DataNode.hh"

namespace ORCA{
	namespace DataFormat{

		struct JSONNode : public DataNode{
			JSONNode(const std::string& name) : DataNode(name) {}
		};
	}
}


#endif //File Guardian