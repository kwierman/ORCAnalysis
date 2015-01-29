#ifndef Deuterium_FileSystem_hh_
#define Deuterium_FileSystem_hh_

#include "Deuterium/FileSystem/Directory.hh"
#include "Deuterium/FileSystem/File.hh"
#include "Deuterium/FileSystem/FileUtilities.hh"

namespace Deuterium{

	//! Includes file and directory 
	namespace FileSystem{
		/*!
			The FileSystem is meant to be used in the following ways

			//If you want to get the current directory, do something like this:
			Directory thisDir('.');
			std::cout<<thisDir.getAbsolutePath()<<std::endl;
			///get the contents of this directory


		 */
	}
}

#endif //File Guardian