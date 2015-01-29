#ifndef ORCAFormat_h_
#define ORCAFormat_h_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


namespace ORCA{
	namespace DataFormat{
		class ORCAFormat{
		public:
			ORCAFormat(std::string& filepath);
			virtual ~ORCAFormat();
			std::string* GetHeader(){return fHeader;}
			void Initialize();
			bool Read(void* buffer, size_t len);
			bool GetNextData(unsigned int& dataId, std::vector<unsigned int>& data);
		private:
			std::ifstream* fInput;
			std::string* fHeader;
			bool fIsBigEndian;
			unsigned int fCurrentWord;
		};
	}
}

#endif //ORCAFormat_h_
