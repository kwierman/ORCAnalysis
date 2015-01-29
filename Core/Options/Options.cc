#include "Deuterium/ORCA/ORCA.hh"



Deuterium::ORCA::OptionList::OptionList(int argc, char* argv[]){
	program_name = argv[0];
	for(int i=1; i<argc; i++){
		raw.push_back(argv[i]);
	}
	if(argc!= raw.size()+1){
		std::cout<<"WARNING: Arguments do not match raw data"<<std::endl;
		std::cout<<"\targc: "<<argc<<std::endl;
		std::cout<<"\traw: "<<raw.size()<<std::endl;
	}
	this->setOption('h',"help",0,false,"","Print this help statement");
}

void Deuterium::ORCA::OptionList::setOption(const char& short_opt,
	const std::string& long_opt,
	const unsigned& n_args=0,
	const bool& required=false,
	const std::string& default_value="",
	const std::string& help_statement=""){
	Option opt(long_opt, default_value, "", help_statement, n_args, required);
	ORCA.insert(std::make_pair(short_opt, opt) );

}


void Deuterium::ORCA::OptionList::parse(){
	//	The new version
	for(str_vec_it it = raw.begin(); it!= raw.end(); ++it){
		if( isOption( *it ) )
			ParseOption( it);
		else
			additional_arguments.push_back( (*it));
	}
	//check the list to see if anything is required, but now met
	for(map_it it = ORCA.begin(); it!= ORCA.end(); it++){
		if( it->second.required && !(it->second.was_set)){
			std::cout<<"Error Option required: "<<it->first<<std::endl;
			exit(-1);
		}
	}
	//anything else that is not an option gets put into the additional arguements

}

void Deuterium::ORCA::OptionList::printUsage(){
	
	std::cout<<"Usage: "<<this->program_name<<" [[<-short_option <option_value> >] [<--long_option <option_value> >] [remaining parameters]]"<<std::endl;
	std::cout<<"ORCA:"<<std::endl;
	for(map_it it = ORCA.begin(); it!= ORCA.end(); ++it){
		std::cout<<"-"<<it->first<<"\t"<<
		"--"<<it->second.long_opt<<"\t";
		if(it->second.required)
			std::cout<<"REQUIRED";
		else
			std::cout<<"OPTIONAL";
		std::cout<<"\t"<<it->second.default_value<<"\t"<<it->second.help_statement<<std::endl;
	}	
}


Deuterium::ORCA::Option Deuterium::ORCA::OptionList::getOption(const char& short_opt){
	return ORCA.find(short_opt)->second ;
}

unsigned Deuterium::ORCA::OptionList::getNAdditionalArguements(){
	return additional_arguments.size();
}
std::string Deuterium::ORCA::OptionList::getAdditionalArguement(const unsigned& i){
	return additional_arguments.at(i);
}


//! Checks to see if this is an option
bool Deuterium::ORCA::OptionList::isOption(const std::string& input){
	if(input.size()>1)
		return input[0]=='-';
	return false;
}

//! Checks to see if this is a long option
bool Deuterium::ORCA::OptionList::isLongOption(const std::string& input){
	if(input.size()>2)
		return isOption(input) && input[1]=='-';
	return false;
}

//! Looks to see if this is an option
void Deuterium::ORCA::OptionList::ParseOption(str_vec_it& it){
	//branch on long option or short option.
	if(isLongOption(*it ) ){
		std::string long_opt = (*it).substr(2);
		for(map_it mit = ORCA.begin(); mit!= ORCA.end() ; ++mit){
			if( mit->second.long_opt == long_opt ){
				mit->second.was_set = true;
				ConsumeArguements(mit, it);
				return;
			}
		}
		std::cout<<"Warning: Unknown long option found: "<<long_opt<<std::endl;
		printUsage();
	}
	else{
		std::string short_opt = (*it).substr(1);
		for(int i=0; i< short_opt.size(); i++){
			map_it mit = ORCA.find( short_opt[i] );
			if(mit == ORCA.end() ){
				std::cout<<"Warning: Unkown short option: "<<short_opt[i];
				continue;
			}
			if(mit->first == 'h'){
				printUsage();
				exit(0);
			}
			mit->second.was_set= true;
			if(i==short_opt.size()-1)
				ConsumeArguements(mit, it);
		}
	}

}

void Deuterium::ORCA::OptionList::ConsumeArguements(map_it& mit, str_vec_it& sit){
	for(int n_args_consumed=0;n_args_consumed < mit->second.n_args; ++n_args_consumed ){
		//consume the argument
		str_vec_it tit = sit;
		tit++;
		if(tit==raw.end()){
			std::cout<<"Warning: Expected Arguments and reached end of input"<<std::endl;
			return;
		}
		if(mit->second.set_value.size()>0)
			mit->second.set_value.append(" ");
		mit->second.set_value.append(*tit);
		this->raw.erase(tit);
	}
}


