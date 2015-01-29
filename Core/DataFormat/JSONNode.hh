#ifndef Deuterium_JSONNode_hh_
#define Deuterium_JSONNode_hh_

#include "Deuterium/DataFormat/DataNode.hh"

namespace Deuterium{
	namespace DataFormat{

		struct JSONNode : public DataNode{
			JSONNode(const std::string& name) : DataNode(name) {}
		};
	}
}


#endif //File Guardian