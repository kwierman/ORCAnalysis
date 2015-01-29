#ifndef Deuterium_Directory_h_
#define Deuterium_Directory_h_


#include <string>
#include <vector>
#include <dirent.h>
#include <sstream>

#include "Deuterium/FileSystem/File.hh"
#include "Deuterium/FileSystem/FileUtilities.hh"

namespace Deuterium {

	namespace FileSystem {


		//! Implementation of directory methods
		class Directory{

			//! Holds content directory information
			std::vector<Directory> fDirEntries;

			//! Holds content file information
			std::vector<File> fFileEntries;

			//! Directory Name
			std::string fDirName;

		public:
			//! Default directory constructor
			Directory();

			//! Construct directory with name
			Directory(const std::string& dirName );

			Directory(const Directory& other){
				fDirName = other.GetName();
			}

			~Directory();

			std::string GetName() const;

			//! Determines validity
			bool IsValid();
		    bool Read();
			unsigned GetNDirs();
			unsigned GetNFiles();
			File FileAt(unsigned i);
			Directory DirAt(unsigned i);
			//! Get back the absolute path
			std::string GetAbsolutePath();
			std::string simplifyPath();
		};
	}
}


#endif //File Guardian