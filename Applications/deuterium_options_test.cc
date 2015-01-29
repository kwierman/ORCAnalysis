#include "Deuterium/Options/Options.hh"

#include <iostream>

int main(int argc, char* argv[]){
	Deuterium::Options::OptionList opt(argc, argv);
	opt.setOption('l',"list",2,false,"Something1 Something2","Gets a list of the next two arguments");
	opt.setOption('q',"queue",0,false,"","Mostly just here for a short option");
	opt.setOption('w',"wierd",0,false,"","Just Another Short Option");
	opt.setOption('r',"required",0,true,"","This is a required statement");
	opt.parse();

	Deuterium::Options::Option option = opt.getOption('l');
	std::cout<<"Found the list option: "<<option.long_opt<<std::endl;
	std::cout<<"Arguments: "<<option.set_value<<std::endl;

	std::cout<<"Additional Arguements: "<<opt.getNAdditionalArguements()<<std::endl;
	for(int i=0; i< opt.getNAdditionalArguements();i++){
		std::cout<<"\t "<<opt.getAdditionalArguement(i)<<std::endl;
	}

}