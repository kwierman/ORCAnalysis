#ifndef Protium_Constructable_h_
#define Protium_Constructable_h_

#include <map>
#include <string>

/**
	\file Constructable.hh
	\author Kevin Wierman
	\date September 11, 2013

	\brief Objects for constructing instances from strings

	To be used as follows:

	In class header:
	
	~~~~~~~~
	namespace MyNamespace{
		class MyClass : ConstructablePrototype{
		    REGISTER_DECLARE_TYPE(MyClass, MyGroup);
		}
	}
	~~~~~~~~

	In class body:

	~~~~~~~~
	REGISTER_DEF_TYPE(MyNamespace::MyClass, MyGroup);
	~~~~~~~~

	Then, when you want to construct an instance:

	~~~~~~~~
	MyClass* instance = dynamic_cast<MyClass>(VirtualFactory<MyGroup>.createInstance("MyClass") );
	~~~~~~~~

**/

namespace Protium{
	namespace Design{

		//! Prototype class to be used as a base class for constructible classes
		struct ConstructablePrototype{
			ConstructablePrototype(){}
			virtual ~ConstructablePrototype(){}
		};

		//! A factory for constructing instances from strings
		template<class Group>
		struct VirtualFactory{
			typedef std::map< std::string, ConstructablePrototype*(*)() > map_type;
		
			//! Returns instance of constructible class for a string s
			static ConstructablePrototype* createInstance(const std::string& s, ConstructablePrototype*(*creator)()=0 ){
				//! \todo: See if we can remove the creator field
				map_type::iterator it = getMap()->find(s);
				if( it==getMap()->end() ){
					return NULL;
				}
				return it->second();
			}
		protected:
			//! Accesses the map field for all classes of group, Group.
			static map_type* getMap(){
				static map_type* map = new map_type();
				return map;
			}
		};

		//! Register used to insert mappings from string to class constructor
		template<class T, class Group>
		struct DerivedRegister : VirtualFactory<Group>{
			//! Function signature for creating instances of classes
			static ConstructablePrototype* create() {return (new T()); }

			//! Definition of insertion of class into map
			DerivedRegister(const std::string& s ){
				getMap->insert(std::make_pair(s, &create) );
			}

		};
	}
}

//! Declares a register for a given constructible class
#define REGISTER_DEC_TYPE(NAME, GROUP) \
    static Protium::Design::DerivedRegister<NAME, GROUP> reg;

//! Defines a register for a given constructible class
#define REGISTER_DEF_TYPE(NAME, GROUP) \
    Protium::Design::DerivedRegister<NAME, GROUP> NAME::reg(#NAME);

#endif //File Guardian
