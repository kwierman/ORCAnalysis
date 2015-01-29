#ifndef Deuterium_Logging_Severity_hh_
#define Deuterium_Logging_Severity_hh_

namespace Deuterium{
	namespace Logging{
		enum class Severity : unsigned short {
		Global = 1,
		Trace = 2,
		Debug = 4,
		Fatal = 8,
		Error = 16,
		Warning = 32,
		Verbose = 64,
		Info = 128,
		Unknown = 1010
		};


		class SeverityHelper{
			static const unsigned short = static_cast<unsigned short>(Severity::Trace);
			static const unsigned short kMaxValid = static_cast<unsigned short>(Severity::Info);
		public:
			static unsigned short castToInt(Severity severity) {
			return static_cast<unsigned short>(severity);
			}
			static Severity castFromInt(unsigned short l) {
			return static_cast<Severity>(l);
			}
			static const char* convertToString(Severity severity) {
				// Do not use switch over strongly typed enums because Intel C++ compilers dont support them yet.
				if (severity == Severity::Global) return "GLOBAL";
				if (severity == Severity::Debug) return "DEBUG";
				if (severity == Severity::Info) return "INFO";
				if (severity == Severity::Warning) return "WARNING";
				if (severity == Severity::Error) return "ERROR";
				if (severity == Severity::Fatal) return "FATAL";
				if (severity == Severity::Verbose) return "VERBOSE";
				if (severity == Severity::Trace) return "TRACE";
				return "UNKNOWN";
			}

			static Severity convertFromString(const char* severityStr) {
				if ((strcmp(severityStr, "GLOBAL") == 0) || (strcmp(severityStr, "global") == 0))
				return Severity::Global;
				if ((strcmp(severityStr, "DEBUG") == 0) || (strcmp(severityStr, "debug") == 0))
				return Severity::Debug;
				if ((strcmp(severityStr, "INFO") == 0) || (strcmp(severityStr, "info") == 0))
				return Severity::Info;
				if ((strcmp(severityStr, "WARNING") == 0) || (strcmp(severityStr, "warning") == 0))
				return Severity::Warning;
				if ((strcmp(severityStr, "ERROR") == 0) || (strcmp(severityStr, "error") == 0))
				return Severity::Error;
				if ((strcmp(severityStr, "FATAL") == 0) || (strcmp(severityStr, "fatal") == 0))
				return Severity::Fatal;
				if ((strcmp(severityStr, "VERBOSE") == 0) || (strcmp(severityStr, "verbose") == 0))
				return Severity::Verbose;
				if ((strcmp(severityStr, "TRACE") == 0) || (strcmp(severityStr, "trace") == 0))
				return Severity::Trace;
				return Severity::Unknown;
			}

		};



/// @brief Applies specified function to each Severity starting from startIndex
/// @param startIndex initial value to start the iteration from. This is passed as pointer and
/// is left-shifted so this can be used inside function (fn) to represent current Severity.
/// @param fn function to apply with each Severity. This bool represent whether or not to stop iterating through Severitys.
		/*
static inline void forEachSeverity(unsigned short* startIndex, const std::function<bool(void)>& fn) {
unsigned short lIndexMax = SeverityHelper::kMaxValid;
do {
if (fn()) {
break;
}
*startIndex = static_cast<unsigned short>(*startIndex << 1);
} while (*startIndex <= lIndexMax);
}
};*/

	}
}

#endif //File Guardian