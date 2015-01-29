#ifndef ORCA_FileSystem_hh_
#define ORCA_FileSystem_hh_

#include "ORCA/FileSystem/Directory.hh"
#include "ORCA/FileSystem/File.hh"
#include "ORCA/FileSystem/FileUtilities.hh"

namespace ORCA{

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