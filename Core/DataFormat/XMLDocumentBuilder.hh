#ifndef Deuterium_XMLDocumentBuilder_hh_
#define Deuterium_XMLDocumentBuilder_hh_

#include "Deuterium/DataFormat/XMLNode.hh"
#include "Deuterium/DataFormat/DataReader.hh"
#include "Deuterium/DataFormat/DataNode.hh"


#include "Protium/Allocation/SmallObject.hh"

#include <iostream> //for debug
#include <ctype.h> //for isspace



namespace Deuterium{
	namespace DataFormat{
		class XMLDocumentReader;

		class XMLVisitor {
		protected:
			XMLDocumentReader* fRoot;
		public:
			XMLVisitor(XMLDocumentReader* root);
		};

		struct OnTagOpenVisitor : public XMLVisitor, CharacterVisitor{
			OnTagOpenVisitor(XMLDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};
		struct OnTagCloseVisitor : public XMLVisitor, CharacterVisitor{
			OnTagCloseVisitor(XMLDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};
		struct OnTagTerminateVisitor : public XMLVisitor, CharacterVisitor{
			OnTagTerminateVisitor(XMLDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};
		struct OnAttributeSetVisitor  : public XMLVisitor, CharacterVisitor{
			OnAttributeSetVisitor(XMLDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};
		struct OnAttributeValueVisitor  : public XMLVisitor, CharacterVisitor{
			OnAttributeValueVisitor(XMLDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};
		struct OnDTDValueVisitor : public XMLVisitor, CharacterVisitor{
			OnDTDValueVisitor(XMLDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};
		struct OnHyphenVisitor : public XMLVisitor, CharacterVisitor{
			OnHyphenVisitor(XMLDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};

		struct OnCommentVisitor : public XMLVisitor, CharacterVisitor{
			OnCommentVisitor(XMLDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};

		struct OnXMLWhiteSpaceVisitor : public XMLVisitor, CharacterVisitor{
			OnXMLWhiteSpaceVisitor(XMLDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};


		class XMLDocumentReader : public DocumentParser{

		public:
			XMLDocumentReader() : DocumentParser() {

			}
			virtual void Init(){
				this->InsertNext(new OnTagOpenVisitor(this) );
				this->InsertNext(new OnTagCloseVisitor(this) );
				this->InsertNext(new OnTagTerminateVisitor(this) );
				this->InsertNext(new OnAttributeSetVisitor(this) );
				this->InsertNext(new OnAttributeValueVisitor(this) );
				this->InsertNext(new OnDTDValueVisitor(this));
				this->InsertNext(new OnHyphenVisitor(this));
				this->InsertNext(new OnCommentVisitor(this));
				this->InsertNext(new OnXMLWhiteSpaceVisitor(this) );
				this->InsertNext(new NameVisitor(this) );
				this->InsertNext(new NumericVisitor(this) );
				this->InsertNext(new DecimalVisitor(this) );
				//GetProcessorRoot()->Print();
			}

			//here's all the actions that need to be taken
			virtual void OnTagOpen()=0;
			virtual void OnTagClose()=0;
			virtual void OnTagTerminate()=0;
			virtual void OnAttributeSet()=0;
			virtual void OnAttributeValue()=0;
			virtual void OnDTDValue()=0;
			virtual void OnHyphen()=0;
			virtual void OnCommentOpen()=0;
			virtual void OnWhiteSpace(const char&)=0;
		};

		class SAXParser : public XMLDocumentReader{
			enum Stat{

				inPre,

				inDTDTagOpen,
				inDTDTagName,
				inDTDTag,
				inDTDAttributeName,
				inDTDAttribute,
				inDTDAttributeValue,
				inDTDTagClose,

				inCommentBang,
				inCommentOpen1,
				inCommentInterior,
				inCommentClose1,
				inCommentClose2,

				inTagOpen,
				inTagOpenName,
				inTagTerminateName,
				inTagInterior,
				inTagAttributeName,
				inTagAttribute,
				inTagAttributeValue,
				inTagExterior,

			} stat;

			std::string fCurrentTokenName;
			std::string fCurrentTokenValue;

		public:
			SAXParser() : stat(inPre) {
				
			}
			void ResetEverything(){
				this->Reset();//reset 
				this->ResetProcessors();
				fCurrentTokenName.erase();
				fCurrentTokenValue.erase();	
			}

			virtual void Init(){
				this->InsertNext(new OnXMLWhiteSpaceVisitor(this) );
				this->InsertNext(new OnTagOpenVisitor(this) );
				stat=inPre;
			}

			virtual void OnTagOpen(){
				//std::cout<<"In tag open with stat: "<<stat<<std::endl;
				switch (stat){
					
					case inPre://If nothing has happened yet.
					stat=inTagOpen;
					ResetEverything();
					InsertNext(new OnXMLWhiteSpaceVisitor(this) );
					InsertNext(new NameVisitor(this) );//Setup this to receive characters
					InsertNext(new OnDTDValueVisitor(this));//
					InsertNext(new OnCommentVisitor(this));					
					break;

					case inTagExterior://then the current name token is notified
					OnXMLData(fCurrentTokenName);
					stat=inTagOpen;
					ResetEverything();
					InsertNext(new NameVisitor(this) );
					InsertNext(new OnCommentVisitor(this));
					InsertNext(new OnTagTerminateVisitor(this) );
					break;		

					default:
					std::cerr<<"Found \"<\" in wrong place"<<std::endl;
				}
			}
			virtual void OnDTDValue(){
				//std::cout<<"In DTD with stat: "<<stat<<std::endl;
				//std::cout<<"Acceptable values: "<<inTagOpen<<" : "<<inDTDTag<<std::endl;
				switch(stat){
					
					case inTagOpen:
					stat= inDTDTagName;
					ResetEverything();
					InsertNext(new NameVisitor(this) );
					InsertNext(new OnXMLWhiteSpaceVisitor(this) );
					break;
					
					case inDTDTag:
					stat=inDTDTagClose;
					ResetEverything();
					InsertNext(new OnTagCloseVisitor(this));
					InsertNext(new OnXMLWhiteSpaceVisitor(this));
					break;

					default:
					std::cerr<<"DTD Value found in the wrong place"<<std::endl;
				}
			}
			virtual void OnTagClose(){
				//std::cout<<"In tag close with stat: "<<stat<<std::endl;
				switch(stat){

					case inTagOpenName:
					OnXMLBeginTag(fCurrentTokenName);
					stat=inTagExterior;
					break;

					case inCommentClose2://in the closing bracket of the comment
					stat=inTagExterior;
					OnXMLComment(fCurrentTokenName);
					break;

					case inTagTerminateName:
					stat=inTagExterior;
					OnXMLTagClose(fCurrentTokenName);
					break;

					case inTagInterior:
					stat=inTagExterior;
					break;
					case inDTDTagClose:
					stat=inPre;
					break;

					default:
					std::cerr<<"Found close tag in the wrong place"<<std::endl;
				}
				ResetEverything();

				InsertNext(new OnTagOpenVisitor(this));
				InsertNext(new OnXMLWhiteSpaceVisitor(this));
				InsertNext(new NameVisitor(this));
				InsertNext(new NumericVisitor(this));
				InsertNext(new DecimalVisitor(this));
				InsertNext(new CommaVisitor(this));
			}

			virtual void OnTagTerminate(){
				//std::cout<<"In tag terminate with stat: "<<stat<<std::endl;
				switch(stat){
					case inTagOpen://case where this is 
					stat=inTagTerminateName;
					ResetEverything();
					InsertNext(new NameVisitor(this));
					InsertNext(new NumericVisitor(this));
					InsertNext(new OnTagCloseVisitor(this));
					break;

					case inTagInterior://then this has already been called
					stat=inTagTerminateName;
					ResetEverything();
					InsertNext(new OnTagCloseVisitor(this));
					break;

					case inTagOpenName:
					OnXMLBeginTag(fCurrentTokenName);
					stat=inTagTerminateName;
					ResetEverything();
					InsertNext(new OnTagCloseVisitor(this));
					break;

					default:
					std::cerr<<"Found a tag terminator in the wrong place"<<std::endl;
				}
			}
			// this should be the equals sign
			virtual void OnAttributeSet(){
				//std::cout<<"In attribute set with stat: "<<stat<<std::endl;
				switch(stat){
					case inDTDAttributeName:
					stat=inDTDAttribute;
					break;

					case inTagAttributeName:
					stat=inTagAttribute;
					break;

					default:
					std::cerr<<"Found \"=\" where is was not expected"<<std::endl;
				}
				this->Reset();//reset 
				this->ResetProcessors();
				fCurrentTokenValue.erase();	
				this->InsertNext(new OnAttributeValueVisitor(this));
			}
			//this should be the " signs
			virtual void OnAttributeValue(){
				switch(stat){

					case inDTDAttribute://case of last one was a equals sign in dtd
					stat=inDTDAttributeValue;
					InsertNext(new NameVisitor(this));
					InsertNext(new NumericVisitor(this));
					InsertNext(new DecimalVisitor(this));
					InsertNext(new CommaVisitor(this));
					InsertNext(new OnAttributeValueVisitor(this) );
					break;

					case inDTDAttributeValue://case of last one was a 
					OnXMLDTD(fCurrentTokenName, fCurrentTokenValue);
					ResetEverything();
					InsertNext(new OnXMLWhiteSpaceVisitor(this));
					InsertNext(new OnDTDValueVisitor(this));
					InsertNext(new NameVisitor(this));
					stat=inDTDTag;
					break;

					case inTagAttribute:
					stat=inTagAttributeValue;
					InsertNext(new NameVisitor(this));
					InsertNext(new NumericVisitor(this));
					InsertNext(new DecimalVisitor(this));
					InsertNext(new CommaVisitor(this));
					InsertNext(new OnAttributeValueVisitor(this) );
					break;

					case inTagAttributeValue:
					OnXMLAttribute(fCurrentTokenName, fCurrentTokenValue);
					ResetEverything();
					InsertNext(new OnXMLWhiteSpaceVisitor(this));
					InsertNext(new OnTagCloseVisitor(this) );
					InsertNext(new OnTagTerminateVisitor(this));
					InsertNext(new NameVisitor(this));				
					stat=inTagInterior;
					break;

					default:
					std::cerr<<"Found Attribute delimiter in the wrong place"<<std::endl;
				}
			}

			virtual void OnHyphen(){
				switch(stat){

					case inCommentBang:
					stat=inCommentOpen1;
					ResetEverything();
					this->InsertNext(new OnHyphenVisitor(this));
					break;

					case inCommentOpen1:
					stat=inCommentInterior;
					ResetEverything();
					InsertNext(new OnXMLWhiteSpaceVisitor(this));
					InsertNext(new NameVisitor(this));
					InsertNext(new OnHyphenVisitor(this));
					InsertNext(new OnTagCloseVisitor(this));
					break;


					case inCommentInterior:
					stat=inCommentClose1;
					break;

					case inCommentClose1:
					stat=inCommentClose2;
					break;

					case inTagExterior:
					fCurrentTokenName.push_back('-');
					break;

					case inTagAttributeValue:
					fCurrentTokenValue.push_back('-');
					break;


					case inDTDAttributeValue:
						fCurrentTokenValue.push_back('-');
					break;

					default:
					std::cerr<<"Found a hyphen in the wrong place"<<std::endl;
					break;
				}
			}
			virtual void OnCommentOpen(){
				//std::cout<<"In comment open with stat: "<<stat<<std::endl;
				switch(stat){
					
					case inTagOpen:
					stat=inCommentBang;
					ResetEverything();
					InsertNext(new OnHyphenVisitor(this));
					break;

					default:
					std::cerr<<"BANG! Found comment open in the wrong place"<<std::endl;
				}
			}

			virtual void OnWhiteSpace(const char& c){
				//std::cout<<"In white space with stat: "<<stat<<std::endl;
				switch(stat){
					case inDTDTagName:
					OnXMLDTD(fCurrentTokenName, fCurrentTokenValue);
					stat=inDTDTag;
					ResetEverything();
					InsertNext(new NameVisitor(this));
					InsertNext(new OnAttributeSetVisitor(this));
					break;

					case inTagOpenName:
					OnXMLBeginTag(fCurrentTokenName);
					stat=inTagInterior;
					InsertNext(new NameVisitor(this));
					InsertNext(new OnAttributeSetVisitor(this));
					break;

					default://default is to do nothing
					fCurrentTokenName.push_back(c);
					return;
				}
			}

			virtual void Notify(CharacterProcessor* c){
				//std::cout<<"In notify with stat: "<<stat<<" With Character: "<<c->GetChar()<<std::endl;
				char ch = c->GetChar();
				switch(stat){
					
					case inDTDTagOpen://in case we're opening a new tag
					stat=inDTDTagName;
					fCurrentTokenName.push_back(ch);
					InsertNext(new NumericVisitor(this));
					break;
					
					case inDTDTagName:
					fCurrentTokenName.push_back(ch);
					break;

					case inDTDTag:
					stat=inDTDAttributeName;
					ResetEverything();
					fCurrentTokenName.push_back(ch);
					InsertNext(new NameVisitor(this));
					InsertNext(new NumericVisitor(this));
					InsertNext(new OnAttributeSetVisitor(this) );
					break;

					case inDTDAttributeName:
					fCurrentTokenName.push_back(ch);
					break;

					case inDTDAttributeValue:
					fCurrentTokenValue.push_back(ch);
					break;

					case inCommentInterior:
					fCurrentTokenName.push_back(ch);
					break;

					case inTagOpen:
					stat=inTagOpenName;
					ResetEverything();
					fCurrentTokenName.push_back(ch);
					InsertNext(new NameVisitor(this));
					InsertNext(new NumericVisitor(this));
					InsertNext(new OnXMLWhiteSpaceVisitor(this));
					InsertNext(new OnTagTerminateVisitor(this) );
					InsertNext(new OnTagCloseVisitor(this));
					break;

					case inTagOpenName:
					fCurrentTokenName.push_back(ch);
					break;

					case inTagTerminateName:
					fCurrentTokenName.push_back(ch);
					break;

					case inTagInterior://if we're in the interior, then this is the 
					stat=inTagAttributeName;
					ResetEverything();	
					fCurrentTokenName.push_back(ch);
					InsertNext(new NameVisitor(this));
					InsertNext(new NumericVisitor(this));
					InsertNext(new OnAttributeSetVisitor(this) );
					break;

					case inTagAttributeName:
					fCurrentTokenName.push_back(ch);
					break;

					case inTagAttributeValue:
					fCurrentTokenValue.push_back(ch);
					break;

					case inTagExterior:
					fCurrentTokenName.push_back(ch);
					break;

					case inCommentClose1:
					fCurrentTokenName.push_back('-');
					fCurrentTokenName.push_back(ch);
					break;

					case inCommentClose2:
					fCurrentTokenName.push_back('-');
					fCurrentTokenName.push_back('-');
					fCurrentTokenName.push_back(ch);
					break;

					default:
					fCurrentTokenName.push_back(ch);
				}
			}

			//THIS IS THE API Definitions
			virtual void OnXMLDTD(const std::string& name, const std::string& value ){
				std::cout<<"In DTD Tag with name: "<<name<<" , value: "<<value<<std::endl;
			}
			virtual void OnXMLBeginTag(const std::string& name){
				std::cout<<"On Begin Tag With name: "<<name<<std::endl;
			}
			virtual void OnXMLTagClose(const std::string& name){
				std::cout<<"On Tag Close With name: "<<name<<std::endl;
			}
			virtual void OnXMLAttribute(const std::string& name, const std::string& value){
				std::cout<<"On Attribute: "<<name<<" :=value: "<<value<<std::endl;
			}
			virtual void OnXMLData(const std::string& data){
				std::cout<<"On Data: "<<data<<std::endl;
			}
			virtual void OnXMLComment(const std::string& text){
				std::cout<<"On Comment: "<<text<<std::endl;
			}

		};


		class DOMBuilder : public SAXParser{
			XMLDTDNode fDTDNode;
			XMLElementNode fRoot;
			XMLElementNode* fCurrentElementNode;
			bool isRoot;


		public:
			DOMBuilder() : fDTDNode("Default","Default"), fRoot("root"), isRoot(true){
				fCurrentElementNode = &fRoot;
			}

			XMLDTDNode& GetDTDNode(){
				return fDTDNode;
			}
			XMLElementNode& GetRootNode(){
				return fRoot;
			}

			bool IsClosed(){
				std::cout<<fCurrentElementNode<<std::endl;
				std::cout<<&fRoot<<std::endl;
				return (fCurrentElementNode==0);
			}


			//THIS IS THE API Definitions
			virtual void OnXMLDTD(const std::string& name, const std::string& value ){
				const std::string n = "version";
				const std::string v="encoding";
				if(name==n){
					fDTDNode.SetVersion(value);
					return;
				}
				else if(name==v){
					fDTDNode.SetEncoding(value);
					return;
				}
				else{
					fDTDNode.SetName(name);
				}

			}
			virtual void OnXMLBeginTag(const std::string& name){
				if(isRoot  ){
					fRoot.SetName(name);
					isRoot=false;
				}
				else{
					XMLElementNode node(name);
					if(fCurrentElementNode){
						DataNode* newNode = fCurrentElementNode->AddNodeToChildren(node);
						if(!newNode){
							std::cerr<<"No node was returned...odd"<<std::endl;
						}

						fCurrentElementNode = static_cast<XMLElementNode*>(newNode);
						if(!fCurrentElementNode){
							std::cerr<<"Problems adding new child node with name: "<<name<<std::endl;
						}
					}
				}
			}
			virtual void OnXMLTagClose(const std::string& name){
				std::cout<<name<<" "<<fCurrentElementNode->GetName()<<std::endl;
				if(fCurrentElementNode==&fRoot){
					fCurrentElementNode=0;
				}
				else{
					fCurrentElementNode = static_cast<XMLElementNode*>(fCurrentElementNode->GetParent() );
				}
			}
			virtual void OnXMLAttribute(const std::string& name, const std::string& value){
				if(fCurrentElementNode)
					fCurrentElementNode->AddAttribute(name,value);
			}
			virtual void OnXMLData(const std::string& data){
				if(fCurrentElementNode)
					fCurrentElementNode->AddData(data);
			}
			virtual void OnXMLComment(const std::string& text){
				if(fCurrentElementNode)
					fCurrentElementNode->AddComment(text);
			}


		};

	}
}




#endif //File Guardian





