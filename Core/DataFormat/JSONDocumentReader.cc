#include "Deuterium/DataFormat/JSONDocumentBuilder.hh"


namespace Deuterium{

	namespace DataFormat{
		JSONVisitor::JSONVisitor(JSONDocumentReader* parent) : fParent(parent) {}			


		OnObjectOpenVisitor::OnObjectOpenVisitor(JSONDocumentReader* root) : JSONVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '{') );
		}
		void OnObjectOpenVisitor::Notify(CharacterProcessor* c){
			fParent->OnObjectOpen();
		}					

		OnObjectCloseVisitor::OnObjectCloseVisitor(JSONDocumentReader* root) : JSONVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '}') );
		}
		void OnObjectCloseVisitor::Notify(CharacterProcessor* c){
			fParent->OnObjectClose();
		}					
		OnArrayOpenVisitor::OnArrayOpenVisitor(JSONDocumentReader* root) : JSONVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '[') );
		}
		void OnArrayOpenVisitor::Notify(CharacterProcessor* c){
			fParent->OnArrayOpen();
		}					
		OnArrayCloseVisitor::OnArrayCloseVisitor(JSONDocumentReader* root) : JSONVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, ']') );
		}
		void OnArrayCloseVisitor::Notify(CharacterProcessor* c){
			fParent->OnArrayClose();
		}	
		OnStringOpenVisitor::OnStringOpenVisitor(JSONDocumentReader* root) : JSONVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '\"') );
		}
		void OnStringOpenVisitor::Notify(CharacterProcessor* c){
			fParent->OnStringOpen();
		}					
		OnStringCloseVisitor::OnStringCloseVisitor(JSONDocumentReader* root) : JSONVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '\"') );
		}
		void OnStringCloseVisitor::Notify(CharacterProcessor* c){
			fParent->OnStringClose();
		}

		OnPairDelimiterVisitor::OnPairDelimiterVisitor(JSONDocumentReader* root) : JSONVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, ':') );
		}
		void OnPairDelimiterVisitor::Notify(CharacterProcessor* c){
			fParent->OnPairDelimit();
		}
		
		OnListDelimiterVisitor::OnListDelimiterVisitor(JSONDocumentReader* root) : JSONVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, ':') );
		}
		void OnListDelimiterVisitor::Notify(CharacterProcessor* c){
			fParent->OnListDelimit();
		}


		OnJSONWhiteSpaceVisitor::OnJSONWhiteSpaceVisitor(JSONDocumentReader* root) : JSONVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, 0x20 ) );
			this->GetProcessorRoot()->InsertNext(new CharacterProcessor(this, 0x09) );
			this->GetProcessorRoot()->InsertNext(new CharacterProcessor(this, 0x0a) );
			this->GetProcessorRoot()->InsertNext(new CharacterProcessor(this, 0x0b) );
			this->GetProcessorRoot()->InsertNext(new CharacterProcessor(this, 0x0c) );
			this->GetProcessorRoot()->InsertNext(new CharacterProcessor(this, 0x0d) );
		}
		void OnJSONWhiteSpaceVisitor::Notify(CharacterProcessor* c){
			fParent->OnWhiteSpace(c->GetChar());
		}


	}

}
