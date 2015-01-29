#ifndef Deuterium_JSONDocumentBuilder_hh_
#define Deuterium_JSONDocumentBuilder_hh_

#include "Deuterium/DataFormat/JSONNode.hh"
#include "Deuterium/DataFormat/DataReader.hh"
#include "Deuterium/DataFormat/DataNode.hh"

namespace Deuterium{

	namespace DataFormat{

		class JSONDocumentReader;

		class JSONVisitor{
		protected:
			JSONDocumentReader* fParent;
		public:
			JSONVisitor(JSONDocumentReader* parent) ;
		};

		struct OnObjectOpenVisitor : public JSONVisitor, CharacterVisitor{
			OnObjectOpenVisitor(JSONDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};
		struct OnObjectCloseVisitor : public JSONVisitor, CharacterVisitor{
			OnObjectCloseVisitor(JSONDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};

		struct OnArrayOpenVisitor : public JSONVisitor, CharacterVisitor{
			OnArrayOpenVisitor(JSONDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};
		struct OnArrayCloseVisitor : public JSONVisitor, CharacterVisitor{
			OnArrayCloseVisitor(JSONDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};		
		struct OnStringOpenVisitor : public JSONVisitor, CharacterVisitor{
			OnStringOpenVisitor(JSONDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};

		struct OnStringCloseVisitor : public JSONVisitor, CharacterVisitor{
			OnStringCloseVisitor(JSONDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};				

		struct OnPairDelimiterVisitor: public JSONVisitor, CharacterVisitor{
			OnPairDelimiterVisitor(JSONDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};				

		struct OnListDelimiterVisitor: public JSONVisitor, CharacterVisitor{
			OnListDelimiterVisitor(JSONDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};		

		struct OnJSONWhiteSpaceVisitor : public JSONVisitor, CharacterVisitor{
			OnJSONWhiteSpaceVisitor(JSONDocumentReader* root);
			virtual void Notify(CharacterProcessor* c);
		};


		class JSONDocumentReader : DocumentParser{
			std::string fCurrentString;

		public:
			JSONDocumentReader() {}
			void Init(){
				this->InsertNext(new OnJSONWhiteSpaceVisitor(this) );
				this->InsertNext(new OnObjectOpenVisitor(this) );
			}
			void OnObjectOpen(){
				//object are composed of 
				this->InsertNext(new OnJSONWhiteSpaceVisitor(this) );
				this->InsertNext(new OnStringOpenVisitor(this) ) ;
				this->InsertNext(new OnObjectCloseVisitor(this) );

			}
			void OnObjectClose(){
				//a lot can happen here
			}
			void OnArrayOpen(){
				//this->InsertNext(new )

			}
			void OnArrayClose(){}
			void OnStringOpen(){
				this->InsertNext(new NameVisitor(this) );
				this->InsertNext(new NumericVisitor(this) );
				this->InsertNext(new OnStringCloseVisitor(this) );
			}
			void OnStringClose(){


			}
			void OnPairDelimit(){}
			void OnListDelimit(){}
			void OnWhiteSpace(const char& c){
				//this should really do nothing
			}
			void Notify(CharacterProcessor* c){}

			//NOW FOR THE API

			void OnNewKey(const std::string& s){}
			void OnStartNewObject(){

			}



		};


	}
}




#endif //File Guardian