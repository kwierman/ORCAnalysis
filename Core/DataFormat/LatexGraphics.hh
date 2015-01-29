#ifndef Deuterium_DataFormat_LatexGraphics_hh_
#define Deuterium_DataFormat_LatexGraphics_hh_

namespace Deuterium{
	namespace DataFormat{


		struct PGFNode : public DataNode{
			void toString(ostream& output )=0;
		};


		class Style{
			std::string fStyle;
			std::string fValue;
			std::string toString(){
				std::string output;
				outputs+=fStyle+="="+=fValue;
			}

		};

		// Type can be "draw" "fill" "clip"
		struct PathAttribute : PGFNode{
			PathAttribute(std::string& type)
		};

		struct Path : PGFNode{


			void toString(ostream& output){
				ostream<<"\\path";
				//look for all the children with a type draw type or something
				//now get all the indices
			}
		};

		struct PolyLine : public Path{
			void toString(ostream& output){
				Path::toString(output);
				//look for each of the 

			}
		};




		struct Document : PGFNode{
			void toString(ostream& output){
				output<<"\\begin{tikzpicture}"<<std::endl;

				output<<"\\end{tikzpicture}"<<std::endl;
			}

		};



	}
}


#endif