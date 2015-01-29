
#include "Deuterium/FileSystem/Directory.hh"
#include "Deuterium/FileSystem/File.hh"

//#include "Protium/Utilities/SignalCatching.h"

#include <iostream>

#define DBGMESG std::cout<<__FILE__<<" : "<<__LINE__<<std::endl;


void PrintIndent(const int& n){
	for(int i=0; i< n; i++) std::cout<<"  ";
}

void PrintFile( Deuterium::FileSystem::File& file, const int& indent){
	PrintIndent(indent);
	std::cout<<"File: "<<file.GetName()<<std::endl;
}

void PrintDirectory( Deuterium::FileSystem::Directory& dir, const int& indent){
	dir.Read();
	dir.GetAbsolutePath();
	PrintIndent(indent);
	std::cout<<"Directory: "<<dir.GetAbsolutePath()<<std::endl;
	int nDirs = dir.GetNDirs();
	int nFiles = dir.GetNFiles();


	if(nFiles){
		PrintIndent(indent);
		std::cout<<"Directory: "<<dir.GetName()<<" has "<<nFiles<<" Files"<<std::endl;
		for(int i=0; i<nFiles;i++){
			Deuterium::FileSystem::File nextFile = dir.FileAt(i);
			PrintFile( nextFile, indent+1 );
		}
	}

	if(nDirs){
		PrintIndent(indent);
		std::cout<<"Directory: "<<dir.GetName()<<" has "<<nDirs<<" subdirectories"<<std::endl;
		for(int i=0; i<nDirs;i++) {
			Deuterium::FileSystem::Directory nextDir= dir.DirAt(i);

			PrintDirectory( nextDir, indent+1 );
		}
	}

}



int main(int argc, char* argv[]){
	//Protium::Utilities::SignalCatcher::Instance().Setup();
	//Test file utilities
	{
		Deuterium::FileSystem::Directory currDir("/home/kwierman/Documents");
		PrintDirectory(currDir, 0);
	}
}