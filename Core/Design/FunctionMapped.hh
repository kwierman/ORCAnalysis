#ifndef Protium_FunctionMapped_hh_
#define Protium_FunctionMapped_hh_

#include <map>
#include <string>
#include <cstdlib>

/**
	\file FunctionMapped.hh
	\author Kevin Wierman
	\date Not so long ago

	\brief Maps class functions to strings to be used in object construction from strings.

	Usage:
	~~~~~~~~
	class MyClass : FunctionMapped<MyClass>{
		double myData;
		void setMyData(const double& dat){myData=dat;}
		public:
			MyClass(){
				AddValueToMap("MyData",&MyClass::setMyData);
			}

	};

	//Later on in your code:
	FunctionMappedPrototype* prototype = ...
	prototype->Set("MyData","0.1");
	~~~~~~~~
**/

namespace Protium{
	namespace Design{
		//! Prototype class to be used with CRTP. Don't use.
		class FunctionMappedPrototype{
			virtual ~FunctionMappedPrototype(){}
			virtual void Set(const std::string& key, const std::string& value )=0;
		};

		//!
		template<class Derived>
		class FunctionMapped : public FunctionMappedPrototype{
			typedef void (Derived::*d_func)(double);
			typedef std::map<std::string, d_func> func_map;
			func_map d_config;

			void AddValueToMap(const std::string& value, d_func f){
				d_config.insert(std::make_pair(value,f));
			}
			void Set(const std::string& value, const std::string& value){
				double newValue = std::atof(value.c_str() );
				d_func call = d_config[key];
				if(!call) return;
				Derived* d = dynamic_cast<Derived*>(this);
				(d->*call)(newValue);
			}
		};
	}
}


#endif //File Guardian
