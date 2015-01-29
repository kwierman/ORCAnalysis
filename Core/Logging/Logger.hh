#ifndef Deuterium_Logger_hh_
#define Deuterium_Logger_hh_

namespace Deuterium{

	namespace Logging{
		/*
		 * HOW TO USE DEUTERIUM LOGGING
		 * Logging is done in a tree-like structure, which can be accessed statically. This is supposed to replicate
		 * ~~~~~~~~
		 *   Deuterium::Logging::GetRootLogger();
		 *   //or get a logger by tag
		 *   Deuterium::Logging::GetLoggerByTag("String_Tag");
		 *   //or get a thread specific logger
		 *   Deuterium::Logging::GetThreadRootLogger();
		 * ~~~~~~~~
		 * HOW TO SET LOGGING FORMAT
		 * ~~~~~~~~
		 * Deuterium::Logging::Logger log("My Logger");
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
			this->f_file = new Deuterium::FileSystem::File(fParent::GetLogLocation() + fParent::GetNewName() );
		}

		Logger(Logger* parent, const std::string& tag){
			this->fParent = parent;
			this->f_file = new Deuterium::FileSystem::File(fParent::GetLogLocation() + tag );	
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
		Deuterium::FileSystem::File fOutput;

	};
	class TerminalLogger : Logger{
		std::ostream fOutput;
	};

}


#endif //File Guardian

