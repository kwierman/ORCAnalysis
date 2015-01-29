#ifndef Deuterium_File_hh_
#define Deuterium_File_hh_

#include <fstream>
#include <string>

namespace Deuterium{

	namespace FileSystem{

		class File{
			std::string fName;//! Name of the file
			std::ifstream fFile;
			char currentCharacter;

		public:
			void Init(){
				fFile.open(fName.c_str() );
			}
			File(const File& other) : fName(other.GetName()) {Init();}
			//! Constructor for standard file
			File(const std::string& filepath): fName(filepath){Init();}
			//! Constructor for standard file
			File(const char* filepath) : fName(filepath){Init();}
			//! Constructor for finding a file with filename
			File(const std::string& path, const std::string& filename): fName(filename){Init();}
			//! Constructor for
			virtual ~File(){
				fFile.close();
			}
			//! returns whether or not the file exists and is readable
			bool IsValid(){
				return fFile.is_open();
			}

			char GetNext(){
				fFile.get(&currentCharacter, sizeof(char) );
				return currentCharacter;
			}
			
			File& operator=(const File& other){
				this->fName = other.fName;
				Init();
			}

			std::string GetName()const {return fName;}
		};
	}
}


#endif //Deuterium_File_h_