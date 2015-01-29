#ifndef ORCA_Logger_hh_
#define ORCA_Logger_hh_

namespace ORCA{

	namespace Logging{
		/*
		 * HOW TO USE ORCA LOGGING
		 * Logging is done in a tree-like structure, which can be accessed statically. This is supposed to replicate
		 * ~~~~~~~~
		 *   ORCA::Logging::GetRootLogger();
		 *   //or get a logger by tag
		 *   ORCA::Logging::GetLoggerByTag("String_Tag");
		 *   //or get a thread specific logger
		 *   ORCA::Logging::GetThreadRootLogger();
		 * ~~~~~~~~
		 * HOW TO SET LOGGING FORMAT
		 * ~~~~~~~~
		 * ORCA::Logging::Logger log("My Logger");
		 * log.SetFormat("Something here, which I haven't defined yet");
		 * ~~~~~~~~
		 * HOW TO SET File Lifetime
		 * ~~~~~~~~
		 * log.SetLifetime( Protium::Time::TimeDate::FromFormat("1", "m" ) );
		 * ~~~~~~~~		 
		 * 
		 * \note Logging utilities are setup in a hierarchical format, with the root implementation at it's core
		 */
	}


	class Logger{
		//need to have all the children here
		std::vector<Protium::Shared_Pointer<Logger> > fChildren;

	public:
		//! Default constructor is enabled here to
		Logger(){
			this->fParent = RootLogger::GetInstance();
			this->f_file = new ORCA::FileSystem::File(fParent::GetLogLocation() + fParent::GetNewName() );
		}

		Logger(Logger* parent, const std::string& tag){
			this->fParent = parent;
			this->f_file = new ORCA::FileSystem::File(fParent::GetLogLocation() + tag );	
		}



		//Getters and setters
		std::string GetTag(){
			return fFile->GetName();
		}

		template<typename T>
		void Log(const T& item);
		template<>
		void Log(const std::string& item);


	};


	class RootLoggerImplementation : public Logger{};


	typedef Protium::Singleton::Singleton<RootLoggerImplementation> LoggerSingleton;

	class RootLogger : public LoggerSingleton {};


	class FileLogger : Logger{
		ORCA::FileSystem::File fOutput;

	};
	class TerminalLogger : Logger{
		std::ostream fOutput;
	};

}


#endif //File Guardian

