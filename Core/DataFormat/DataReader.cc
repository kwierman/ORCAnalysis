#include "Deuterium/DataFormat/DataReader.hh"


namespace Deuterium{
	namespace DataFormat{
		CharacterProcessor::CharacterProcessor(CharacterVisitor* Visitor, const char& c) : fVisitor(Visitor), 
			fNext(0), fCharacter(c) {}
		CharacterProcessor::~CharacterProcessor(){Reset();}	
		char CharacterProcessor::GetChar(){return fCharacter;}

		void CharacterProcessor::SetVisitor(CharacterVisitor* Visitor){fVisitor=Visitor;}
		CharacterVisitor* CharacterProcessor::GetVisitor(){return fVisitor;}
		bool CharacterProcessor::SetVisitorForCharacter(CharacterVisitor* v, const char& c){
			if(c==fCharacter){
				this->fVisitor=v;
			}
			else if(fNext){
				fNext->SetVisitorForCharacter(v, c);
			}
		}
		void CharacterProcessor::ClearVisitors(){
			fVisitor=0;
			if(fNext)
				fNext->ClearVisitors();
		}


		void CharacterProcessor::InsertLast(CharacterProcessor* proc){
			if(fNext)
				fNext->InsertLast(proc);
			else
				fNext = proc;
		}
		void CharacterProcessor::InsertNext(CharacterProcessor* proc){
			CharacterProcessor* thisNext = fNext;
			fNext = proc;

			proc->GetLast()->fNext = thisNext;
		}
		CharacterProcessor* CharacterProcessor::GetLast(){
			if(fNext)
				return fNext->GetLast();
			return this;
		}
		void CharacterProcessor::Reset(){
			if(fNext)
				delete fNext;
			fNext=0;
		}
		bool CharacterProcessor::Processes(const char& c){
			if(c==fCharacter){
				if(fVisitor){
					fVisitor->Notify(this);
					return true;
				}
			}
			if(fNext)
				return fNext->Processes(c);
			return false;
		}
	}
}






