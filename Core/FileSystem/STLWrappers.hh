#ifndef ORCA_FileSystem_StlWrappers_hh_
#define ORCA_FileSystem_StlWrappers_hh_

#include <fstream>

namespace ORCA{
	namespace FileSystem{


		class FStreamWrapper{
			std::fstream fInput;
		public:

			/**
				Flags that may be passed are (in the format of std::fstream::<#1>):
				in	input	File open for reading: the internal stream buffer supports input operations.
				out	output	File open for writing: the internal stream buffer supports output operations.
				binary	binary	Operations are performed in binary mode rather than text.
				ate	at end	The output position starts at the end of the file.
				app	append	All output operations happen at the end of the file, appending to its existing contents.
				trunc	truncate	Any contents that existed in the file before it is open are discarded.
			**/
				//The return value is the "openness" state of the file stream
			bool Open(const std::string& name){
				fInput.open(name);
				return fInput.is_open();
			}
			bool InOpen() const{
				return fInput.is_open();
			}
			bool Close(){
				fInput.close();
				return !(fInput.is_open());
			}

			std::fstream& GetInternal(){
				return fInput;
			}

			std::filebuf* GetInternal(){
				return fInput.rdbuf();
			}

			bool IsGood(){
				return fInput.good();
			}


			bool EOF(){
				return fInput.eof();
			}

			bool Fail(){
				return fInput.fail();
			}

			bool Bad(){
				return fInput.bad();
			}

			bool operator!(){
				return !fInput;
			}

			bool operator bool(){
				return bool(fInput);
			}
			void ClearFlags(){
				fInput.clear();
			}
			void Fill(const char& x){
				fInput.fill(x);
			}

		};

		class InputWrapper{
			std::ifstream fInput;
		public:
			bool Open(const std::string& name){
				fInput.open(name);
				return fInput.is_open();
			}

			bool Close(){
				fInut.close();
				return !(fInput.is_open() );
			}

			bool IsOpen(){
				return fInput.is_open();
			}
			std::fstream& GetInternal(){
				return fInput;
			}
			streamsize& Count(){
				return fInput.count();
			}
			std::string GetChars(const unsigned& n){
				char str[n];
				fInput.get(str,n);
				std::string output = str;
				delete[] str;
				return output;
			}
			std::string GetLine(const unsigned& maxChars, const char& delim){
				char str[maxChars];
				fInput.get_line(str,maxChars, delim);
				std::string output = str;
				delete[] str;
				return output;
			}

			void Ignore(const unsigned& n, const char& c){
				fInput.ignore(n,c);
			}
			char peek(){
				return fInput.peek();
			}
			template<typename T>
			std::vector<T> Read(const unsigned& n){
				T buffer[n];
				fInput.read(buffer,n);
				std::vector<T> buffer2
				delete[] buffer;
				return buffer2;
			}
			std::vector<char> ReadSome(const unsigned& s){
				char buffer[n];
				fInput.readsome(buffer, n);
				std::vector<char> buf = buffer;
				delete[] buffer;
				return buf;
			}
			void Put(const char& c){
				fInput.putback(c);
			}
			void UnGet(){
				fInput.unget();
			}
			int StreamPos(){
				return fInput.tellg();
			}
			void PutStreamPos(const unsigned& n ){
				fInput.seekg(n);
			}
			void PutStreamPosFromBegin(const unsigned& n){
				fInput.seekg(n, std::ios_base::beg);
			}
			void PutStreamPosFromEnd(const unsigned& n){
				fInput.seekg(n, std::ios_base::end);
			}
			void PutStreamPosFromCurrentPosition(const unsigned& n){
				fInput.seekg(n, std::ios_base::cur);
			}
		};


		class OutputWrapper{
			std::ofstream fOutput;
		public:
			bool Open(const std::string& name){
				fOutput.open(name);
				return fOutput.is_open();
			}
			bool Close(){
				fOutput.close();
				return (!fOutput.is_open());
			}
			std::ofstream& GetInternal(){
				return fOutput;
			}
			void putChar(const char& c){
				fOutput.put(c);
			}
			void Write(std::vector<char>& buff){
				fOutput.write(buff.data, buff.length());
			}
			int StreamPos(){
				return fOutput.tellg();
			}
			void PutStreamPos(const unsigned& n ){
				fOutput.seekg(n);
			}
			void PutStreamPosFromBegin(const unsigned& n){
				fOutput.seekg(n, std::ios_base::beg);
			}
			void PutStreamPosFromEnd(const unsigned& n){
				fOutput.seekg(n, std::ios_base::end);
			}
			void PutStreamPosFromCurrentPosition(const unsigned& n){
				fOutput.seekg(n, std::ios_base::cur);
			}

			void Flush(){
				fOutput.flush();
			}

		};

	}
}


#endif //File guardian