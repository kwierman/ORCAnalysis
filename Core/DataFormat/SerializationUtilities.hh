#ifndef Deuterium_SerializationUtilities_hh_
#define Deuterium_SerializationUtilities_hh_

#include <string>

namespace Deuterium{
	namespace DataFormat{


		//String Functionality
		template<typename XType>
		struct AsString{
			std::string operator()(const XType& other){
				//this should be a lexical cast
				std::stringstream ss;
				ss << other;
				return ss.str();
			}
		};

		struct String2Int{
			int operator(const std::string& other)(){	
				return std::stoi(other);
			}
		};

		struct String2Double{
			int operator(const std::string& other)(){	
				return std::stoi(other);
			}
		};

		struct String2Bool{
			int operator(const std::string& other)(){	
				return std::stoi(other);
			}
		};


		//int float
		//-----------------------------------------------
		struct Int2Double{
			double operator(const int& other)(){	
				return double(other);
			}
		};

		struct Double2Int{
			int operator(const double& other)(){	
				return int(other+0.5);
			}
		};
		//-----------------------------------------------

		//float to string

		//string double
		//-----------------------------------------------
		struct Double2String{
			std::string operator(const double& other)(){	
				return std::string::to_string(other);
			}
		};

		struct String2Int{
			int operator(const std::string& other)(){	
				return std::stoi(other);
			}
		};
		//-----------------------------------------------

	}
}


#endif //File Guardian