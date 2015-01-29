#include "Deuterium/FileSystem/FileUtilities.hh"

#include <string>//! for returning the string representation of path
#include <unistd.h>//! 
#include <stdio.h>
;



static inline unsigned Deuterium::FileSystem::SwapWord(const unsigned& word){
    return (
        ((word & 0xff000000) >> 24) |
        ((word & 0x00ff0000) >> 8) |
        ((word & 0x0000ff00) << 8) |
        ((word & 0x000000ff) << 24)
    );
}


std::string Deuterium::FileSystem::cwd(){
	char cCurrentPath[FILENAME_MAX];

	if (!getcwd(cCurrentPath, sizeof(cCurrentPath))){
	  return "ERROR: Ask Kevin to fix this";
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	return std::string(cCurrentPath);
}