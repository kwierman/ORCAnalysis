#ifndef Protium_DumbPointer_hh_
#define Protium_DumbPointer_hh_

/**
	\file DumbPointer
	\author Kevin Wierman
	\brief Illustrates basic semantics of Smart Pointer functionality without any of the supporting code.
	\warning Do not use. This object will destroy the host object on deletion, making this a terrible smart pointer.
	\warning In addition, this ``smart pointer'' will also deallocate the host under reassignment.
**/


namespace Protium{
	namespace SmartPointer{

		/**
			A dumb pointer class to illustrate the semantics of smart pointers.
			\author Kevin Wierman
		**/
		template <class Host>
		class DumbPointer {
			Host* host;
		public:
			DumbPointer(Host* pointee) : host(pointee) {}
			DumbPointer& operator=(const DumbPointer& other){
				delete host;
				host = &(*other);
				return *this;
			}
			~DumbPointer(){
				delete host;
			}

			Host& operator*() const{
				return *host;
			}

			Host* operator->() const{
				return host;
			}
		};

		typedef DumbPointer<unsigned> UIntPtr;
		typedef DumbPointer<int> IntPtr;
		typedef DumbPointer<float> FltPtr;
		typedef DumbPointer<double> DblPtr;
	}
}






#endif //Protium_DumbPointer_hh_