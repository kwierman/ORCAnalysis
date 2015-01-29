#include "File.h"
#include "DeuteriumFormat.h"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <exception>
#include <string>





/*
Deuterium::File::File(std::string& filepath){
	fInput = new std::ifstream(filepath.c_str());
}

Deuterium::File::~File(){
	delete fInput;
    delete fHeader;
}

bool Deuterium::File::Read(void* buffer, size_t size)
{
    if (! (fInput && *fInput))
        return false;
    if(fInput->eof() ) return false;

    int remainingSize = size;
    char* bufferPtr = reinterpret_cast<char*>(buffer);
    while (remainingSize > 0) {
        fInput->read(bufferPtr, remainingSize);
        int readSize = fInput->gcount();
        if (readSize > 0) {
            bufferPtr += readSize;
            remainingSize -= readSize;
        }
        if ((remainingSize > 0) && fInput->eof()) {
            fInput->clear();
            return false;
        }
    }
    return (remainingSize == 0);
}


void Deuterium::File::Initialize(){
	unsigned int headerLen, headerSize;
	bool isBigEndian;
	isBigEndian = this->Read(&headerLen, sizeof(unsigned int) );
	isBigEndian = isBigEndian && this->Read(&headerSize, sizeof(unsigned int) );
	if(!isBigEndian){
		std::cout<<"Not an orca file, or trouble reading from file"<<std::endl;
		throw std::exception();
	}

	//Check for endianness
	if (headerLen & 0xffff0000) {
        fIsBigEndian = true;
        headerLen = SwapWord(headerLen);
        headerSize = SwapWord(headerSize);
    }
    else
        fIsBigEndian = false;

    char* buffer = new char[headerLen*4];
    this->Read(buffer, 4*(headerLen-2));
    buffer[headerSize]='\0';

    std::string header = buffer;

    fHeader = new ORHeader(header);

    delete[] buffer;
}

//Each set of data should contain an id (with length), a crate a card and the rest of the data stored as a vector
bool Deuterium::File::GetNextData(unsigned int& dataId, std::vector<unsigned>& data){
    data.clear();

    //process the first word
    if(!(this->Read(&fCurrentWord, sizeof(unsigned int) )) ){
        return false;
    }
    if(fIsBigEndian)
        fCurrentWord = SwapWord(fCurrentWord);
    if(fCurrentWord & bitShortForm) {
        dataId = (fCurrentWord & maskShortFormDataId) >> shiftShortFormDataId;
        data.push_back( (fCurrentWord & maskShortFormData) >> shiftShortFormData );
    }
    else {
        dataId = (fCurrentWord & maskLongFormDataId) >> shiftLongFormDataId;
        unsigned int length = (fCurrentWord & maskLongFormDataLength) >> shiftLongFormDataLength;
        length -= 1;
        data.resize(length);
        for(std::vector<unsigned>::iterator it = data.begin(); it!=data.end(); ++it){
            if(! this->Read(&fCurrentWord, sizeof(unsigned int))){
                return false;
            }            
            if(fIsBigEndian) fCurrentWord = SwapWord(fCurrentWord);
            (*it) = fCurrentWord;
        }
    }
    return true;
}

*/