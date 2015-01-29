#ifndef Deuterium_DataReader_hh_
#define Deuterium_DataReader_hh_

#include "Protium/Allocation/SmallObject.hh"

#include <iostream>


namespace Deuterium{
	namespace DataFormat{


		//! Contains an action to be executed when a character is reached
		class CharacterVisitor;

		//Derive this for
		class CharacterProcessor : public Protium::Allocation::DefaultSmallObject {
			const char fCharacter;

			//this is not an owned pointer
			CharacterVisitor* fVisitor;//Points to the processing directive

			//this is an owned pointer
			CharacterProcessor* fNext;//Points to the next processor in the chain
		public:
			CharacterProcessor(CharacterVisitor* parent, const char& c) ;
			virtual ~CharacterProcessor();
			
			char GetChar();

			void SetVisitor(CharacterVisitor* Visitor);
			CharacterVisitor* GetVisitor();
			bool SetVisitorForCharacter(CharacterVisitor* v, const char& c);
			void ClearVisitors();


			void InsertLast(CharacterProcessor* proc);
			void InsertNext(CharacterProcessor* proc);
			CharacterProcessor* GetLast();

			void Reset();
			virtual bool Processes(const char& c);
			virtual void Print(){
				std::cout<<"Printing Processor: "<<fCharacter<<std::endl;
				if(fNext)
					fNext->Print();
			}
		};



		class CharacterVisitor{
			CharacterProcessor* fProcRoot;
			CharacterVisitor* fNext;

		public:
			CharacterVisitor() : fNext(0), fProcRoot(0) {}
			virtual ~CharacterVisitor(){
				Reset();
			}
			void Reset(){
				if(fNext)
					delete fNext;
				fNext=0;
			}
			void ResetProcessors(){
				if(fProcRoot)
					delete fProcRoot;
				fProcRoot=0;
			}

			void SetProcessorRoot(CharacterProcessor* proc){fProcRoot=proc;}
			CharacterProcessor* GetProcessorRoot(){return fProcRoot;}
			CharacterVisitor* GetNext(){return fNext;}
			void InsertNext(CharacterVisitor* v){
				CharacterVisitor* v2=fNext;
				fNext=v;
				v->fNext = v2;

				if(fProcRoot){
					fProcRoot->InsertNext(v->GetProcessorRoot() );
				}
				else{
					SetProcessorRoot(fProcRoot = v->GetProcessorRoot() );
				}
			}
			void InsertLast(CharacterVisitor* v){
				if(fNext)
					fNext->InsertLast(v);
				else{
					fNext=v;
					if(fProcRoot){
						fProcRoot->InsertLast(v->GetProcessorRoot() );
					}
					else{
						SetProcessorRoot( v->GetProcessorRoot() );
					}
				}
			}
			virtual void Notify(CharacterProcessor* c){}
		};


		struct UnexpectedCharacterVisitor : public CharacterVisitor{
			virtual void Notify(CharacterProcessor* c){
				std::cerr<<"Unexpected Character: "<<c->GetChar()<<std::endl;
			}
		};

		struct NameVisitor : public CharacterVisitor{
			NameVisitor(CharacterVisitor* visit){
				if(!visit)
					visit=this;
				//setup processors for each of the elements
				std::string lowerCase = "qwertyuiopasdfghjklzxcvbnm";
				std::string upperCase = "QWERTYUIOPASDFGHJKLZXCVBNM";
				//std::string numeric = "1234567890";
				SetProcessorRoot(new CharacterProcessor(visit, lowerCase[0]));
				for(unsigned int i = 1; i<lowerCase.length(); i++) {
					this->GetProcessorRoot()->InsertNext( new CharacterProcessor(visit, lowerCase[i] ) );
				}
				for(unsigned int i = 0; i<upperCase.length(); i++) {
					this->GetProcessorRoot()->InsertNext( new CharacterProcessor(visit, upperCase[i] ) );
				}
				//for(unsigned int i = 0; i<numeric.length(); i++) {
					//this->GetProcessorRoot()->InsertNext( new CharacterProcessor(this, numeric[i] ) );
				//}		
			}
			~NameVisitor(){

			}
			virtual void Notify(CharacterProcessor* c){
				//this is where the document builder
			}			
		};
		struct NumericVisitor : public CharacterVisitor{
			NumericVisitor(CharacterVisitor* visit){
				if(!visit)
					visit=this;
				//setup processors for each of the elements
				std::string lowerCase = "11234567890";
				//std::string numeric = "1234567890";
				SetProcessorRoot(new CharacterProcessor(visit, lowerCase[0]));
				for(unsigned int i = 1; i<lowerCase.length(); i++) {
					this->GetProcessorRoot()->InsertNext( new CharacterProcessor(visit, lowerCase[i] ) );
				}

			}
			virtual void Notify(CharacterProcessor* c){
				//this is where the document builder
			}			
		};
		struct DecimalVisitor : public CharacterVisitor{
			DecimalVisitor(CharacterVisitor* visit){
				if(!visit)
					visit=this;
				//setup processors for each of the elements
		
				//std::string numeric = "1234567890";
				SetProcessorRoot(new CharacterProcessor(visit, '.'));
			}
			virtual void Notify(CharacterProcessor* c){
				//this is where the document builder
			}			
		};
		struct CommaVisitor : public CharacterVisitor{
			CommaVisitor(CharacterVisitor* visit){
				if(!visit)
					visit=this;
				//setup processors for each of the elements
		
				//std::string numeric = "1234567890";
				SetProcessorRoot(new CharacterProcessor(visit, ','));
			}
			virtual void Notify(CharacterProcessor* c){
				//this is where the document builder
			}			
		};

		struct WhiteSpaceVisitor : public CharacterVisitor{
			WhiteSpaceVisitor(CharacterVisitor* visit){
				if(visit==0) visit=this;
				if(!GetProcessorRoot())
					SetProcessorRoot(new CharacterProcessor(visit,' ') );
				else
				this->GetProcessorRoot()->InsertNext(new CharacterProcessor(visit,  ' ') );
				this->GetProcessorRoot()->InsertNext(new CharacterProcessor(visit, '\t') );
				this->GetProcessorRoot()->InsertNext(new CharacterProcessor(visit, '\n') );
				this->GetProcessorRoot()->InsertNext(new CharacterProcessor(visit, '\v') );
				this->GetProcessorRoot()->InsertNext(new CharacterProcessor(visit, '\f') );
				this->GetProcessorRoot()->InsertNext(new CharacterProcessor(visit, '\r') );
			}
			virtual void Notify(CharacterProcessor* c){
				std::cout<<"Houston, we've got a problem"<<std::endl;
			}
		};


		class DocumentParser : public CharacterVisitor {

		public:
			DocumentParser(){

			}
			virtual ~DocumentParser(){
			}

			void Process(const char& c){
				if(!GetProcessorRoot()){
					std::cerr<<"Requested Processing with no processors"<<std::endl;
					return;
				}
				if( GetProcessorRoot()->Processes(c) ){
					//std::cout<<"Successful processing of: "<<c<<std::endl;
				}
				else{
					std::cerr<<"Encountered Unknown Character: \""<<c<<"\""<<std::endl;
					//GetProcessorRoot()->Print();
				}
			}
		};
	}
}


#endif//Deuterium_DataReader_hh_