#ifndef Deuterium_FileUtilities_h_
#define Deuterium_FileUtilities_h_

#include <string>

namespace Deuterium{

	namespace FileSystem{




		static inline unsigned SwapWord(const unsigned& word);


		static const unsigned bitShortForm = 0x80000000;
		static const unsigned maskShortFormDataId = 0xfc000000;
		static const unsigned shiftShortFormDataId = 0;
		static const unsigned maskLongFormDataId = 0xfffc0000;
		static const unsigned shiftLongFormDataId = 0;
		static const unsigned maskShortFormData = 0x03ffffff;
		static const unsigned shiftShortFormData = 0;
		static const unsigned maskLongFormDataLength = 0x0003ffff;
		static const unsigned shiftLongFormDataLength = 0;


		std::string cwd();


	}
}



#endif //File Guardian