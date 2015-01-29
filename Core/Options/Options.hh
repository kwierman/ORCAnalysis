#ifndef Deuterium_Options_h_
#define Deuterium_Options_h_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cstdlib>

namespace Deuterium{
	namespace Options{

/**
	To be used as follows:
	~~~~~~~~~~~~~~~~
	int main(int argc, char* argv[]){
		Deuterium::Options::OptionList opt(argc, argv);
		opt.setOption('l',"list","",false);
		opt.parse();
		opt.printUsage();
	}
	~~~~~~~~~~~~~~~~
**/

		//!TODO: move this to the interior of the option list.
		struct Option{
			//char short_opt; (already included in map)
			std::string long_opt;
			bool was_set;
			std::string default_value;
			std::string set_value;
			std::string help_statement;
			int n_args;
			bool required;

			Option(std::string lopt, std::string def, std::string set, std::string help, int n, bool req) :
			long_opt(lopt), default_value(def), set_value(set), help_statement(help), n_args(n), required(req), was_set(false) {}
			Option(const Option& other){
				long_opt = other.long_opt;
				was_set = other.was_set;
				default_value = other.default_value;
				set_value = other.set_value;
				help_statement = other.help_statement;
				n_args = other.n_args;
				required = other.required;
			}
			Option& operator=(const Option& other){
				long_opt = other.long_opt;
				was_set = other.was_set;
				default_value = other.default_value;
				set_value = other.set_value;
				help_statement = other.help_statement;
				n_args = other.n_args;
				required = other.required;
				return *this;				
			}

		};

		class OptionList{

			//! holds the raw data
			std::vector<std::string> raw;
			typedef std::vector<std::string>::iterator str_vec_it;
			
			//! holds the options
			std::map<char, Option> options;
			typedef std::map<char, Option>::iterator map_it;

			//! The program name (for printing help statement)
			std::string program_name;
			//! Additional Arguements handed to the program
			std::vector<std::string> additional_arguments;

		public:
			//! Construct the argument list from the program arguments
			OptionList(int argc, char* argv[]);

			//! Set an option to be parsed
			//! \param short_opt ShortOptionValue a character representing a short option. i.e, 'v' for -v
			//! \param long_opt LongOptionValue a string representing a long option. i.e. "verbose" for --verbose
			//! \param n_args NumberofArguments Integer number of arguments to hand to option
			void setOption(const char& short_opt,
				const std::string& long_opt,
				const unsigned& n_args,
				const bool& required,
				const std::string& default_value,
				const std::string& help_statement);

			//! Parse the program input for options
			void parse();

			void printUsage();
			Option getOption(const char& short_opt);


			unsigned getNAdditionalArguements();
			std::string getAdditionalArguement(const unsigned& i);


		private://Helper Functions
			//! Checks to see if this is an option
			bool isOption(const std::string& input);
			//! Checks to see if this is a long option
			bool isLongOption(const std::string& input);
			//! Looks to see if this is an option
			void ParseOption(str_vec_it& it);

			void ConsumeArguements(map_it& mit, str_vec_it& sit);


		};
	}
}

#endif //File Guardian

