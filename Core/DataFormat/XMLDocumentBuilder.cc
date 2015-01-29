#include "Deuterium/DataFormat/XMLDocumentBuilder.hh"

namespace Deuterium{
	namespace DataFormat{
		XMLVisitor::XMLVisitor(XMLDocumentReader* root) : fRoot(root) {}		

		OnTagOpenVisitor::OnTagOpenVisitor(XMLDocumentReader* root) : XMLVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '<') );
		}
		void OnTagOpenVisitor::Notify(CharacterProcessor* c){
			fRoot->OnTagOpen();
		}					

		OnTagCloseVisitor::OnTagCloseVisitor(XMLDocumentReader* root) : XMLVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '>') );
		}
		void OnTagCloseVisitor::Notify(CharacterProcessor* c){
			fRoot->OnTagClose();
		}

		OnTagTerminateVisitor::OnTagTerminateVisitor(XMLDocumentReader* root) : XMLVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '/') );
		}
		void OnTagTerminateVisitor::Notify(CharacterProcessor* c){
			fRoot->OnTagTerminate();
		}

		OnAttributeSetVisitor::OnAttributeSetVisitor(XMLDocumentReader* root) : XMLVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '=') );
		}
		void OnAttributeSetVisitor::Notify(CharacterProcessor* c){
			fRoot->OnAttributeSet();
		}	

		OnAttributeValueVisitor::OnAttributeValueVisitor(XMLDocumentReader* root) : XMLVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '\"') );
		}
		void OnAttributeValueVisitor::Notify(CharacterProcessor* c){
			fRoot->OnAttributeValue();
		}

		OnDTDValueVisitor::OnDTDValueVisitor(XMLDocumentReader* root) : XMLVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '?') );
		}
		void OnDTDValueVisitor::Notify(CharacterProcessor* c){
			fRoot->OnDTDValue();
		}

		OnHyphenVisitor::OnHyphenVisitor(XMLDocumentReader* root) : XMLVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '-') );
		}
		void OnHyphenVisitor::Notify(CharacterProcessor* c){
			fRoot->OnHyphen();
		}

		OnCommentVisitor::OnCommentVisitor(XMLDocumentReader* root) : XMLVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, '!') );
		}
		void OnCommentVisitor::Notify(CharacterProcessor* c){
			fRoot->OnCommentOpen();
		}

		OnXMLWhiteSpaceVisitor::OnXMLWhiteSpaceVisitor(XMLDocumentReader* root) : XMLVisitor(root){
			SetProcessorRoot(new CharacterProcessor(this, 0x20 ) );
			this->GetProcessorRoot()->InsertNext(new CharacterProcessor(this, 0x09) );
			this->GetProcessorRoot()->InsertNext(new CharacterProcessor(this, 0x0a) );
			this->GetProcessorRoot()->InsertNext(new CharacterProcessor(this, 0x0b) );
			this->GetProcessorRoot()->InsertNext(new CharacterProcessor(this, 0x0c) );
			this->GetProcessorRoot()->InsertNext(new CharacterProcessor(this, 0x0d) );
		}
		void OnXMLWhiteSpaceVisitor::Notify(CharacterProcessor* c){
			fRoot->OnWhiteSpace(c->GetChar());
		}


	}
}
