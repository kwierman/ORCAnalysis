
#include "Protium/SmartPointer/DumbPointer.hh"
#include "Protium/SmartPointer/SmartPointer.hh"

#include <iostream>

#include "Protium/Utilities/SignalCatching.hh"

class PointerTester{
	static unsigned fData;
	unsigned fPersonal;
	public:
		PointerTester(){
			fPersonal=++fData;
			std::cout<<"In constructor with Tester: "<<fPersonal<<std::endl;
		}
		~PointerTester(){
			std::cout<<"In destructor with Tester: "<<fPersonal<<std::endl;
		}
};
	unsigned PointerTester::fData = 0;

using Protium::SmartPointer::UIntPtr;

int main(int argc, char* argv[]){
	Protium::Utilities::SignalCatcher::Instance().Setup();

	UIntPtr ptr(new unsigned );
	Protium::SmartPointer::DumbPointer<PointerTester> ptr2(new PointerTester);
	Protium::SmartPointer::SmartPtr<PointerTester> smrt(new PointerTester);

}