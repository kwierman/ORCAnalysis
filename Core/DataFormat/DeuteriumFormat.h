#ifndef DeuteriumFormat_h_
#define DeuteriumFormat_h_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

/*
namespace Deuterium{
	class DeuteriumFormat{
	public:
		DeuteriumFormat(std::string& filepath);
		virtual ~DeuteriumFormat();
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
*/
#endif //DeuteriumFormat_h_
