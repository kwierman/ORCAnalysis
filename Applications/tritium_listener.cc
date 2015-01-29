


int main(int argc, char* argv[]){

	//set up the option list
	Deuterium::Options::OptionList opt(argc, argv);

	//set up the logging
	Deuterium::Logging::RootLogger::GetInstance().SetRootOutputName("TritiumPortListener");

	//log everything to file


	//Set up the structure of the program
	Tritium::TritiumTree tree;//the analysis tree
	//add in the server input
	tree.Insert(Tritium::Server(1448) ).Insert(Tritium::TritiumWriter("TritiumServer"));
	//And let it run
	tree.Go();



	
}