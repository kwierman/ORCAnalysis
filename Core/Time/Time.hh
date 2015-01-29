#ifndef Protium_Time_hh_
#define Protium_Time_hh_

#include <ctime>
#include <string>
#include <iomanip>

#include "Protium/Time/Clock.hh"

namespace Protium{
	//! Objects that are to be used in timing applications
	namespace Time{

		/**
			Wrapper Functionality for C-Style Time
		**/
		class TimePrimitive{
		protected:
			//! Data
			time_t fTime;
		public:
			TimePrimitive(){fTime = 0;}
			TimePrimitive(const time_t& t) : fTime(t) {}
			TimePrimitive(const TimePrimitive& t) : fTime(t.fTime) {}
			TimePrimitive& operator=(const TimePrimitive& other) {
				fTime = other.fTime;
			}
			virtual ~ TimePrimitive(){}
			inline time_t GetTime() const {return fTime;}
			inline void SetTime(const time_t& t) {fTime=t;}
			static TimePrimitive Now(){
				return TimePrimitive( std::time(NULL) );
			}
			TimePrimitive& operator-=(const TimePrimitive& other){
				this->fTime-=other.fTime;
				return *this;
			}

		};

		//! Extends TimePrimitive to include formatting and time zones
		class TimeDate : public TimePrimitive{
			tm fTM;
		public:
			TimeDate(const time_t& t) : TimePrimitive(t)  {
				time_t temp = this->GetTime();
				tm* temp2 = localtime( &temp );
				fTM = *temp2;
				//delete temp2; 
			}

			TimeDate(	const int& sec=0, 
					const int& min=0, 
					const int& hour =0, 
					const int& day=0, 
					const int& month=0,
					const int& year=0,
					const int& weekday=0,
					const bool& dst=false ) : TimePrimitive( std::time(NULL) ) {
				fTM.tm_sec = sec;
				fTM.tm_min = min;
				fTM.tm_hour = hour;
				fTM.tm_mday = day;
				fTM.tm_mon = month;
				fTM.tm_year = year;
				fTM.tm_wday = weekday;
				fTM.tm_isdst = dst? 1 : 0;
				fTime = mktime(&fTM); 
			}
			TimeDate(const TimeDate& other) : TimePrimitive(std::time(NULL) ) {
				this->fTM = other.fTM;
				this->fTime = mktime(&fTM);
			}

			TimeDate(const TimePrimitive& other) : TimePrimitive(other){
				time_t t = other.GetTime();
				this->fTM = *std::localtime( &t );
			}

			inline time_t GetTimeLocal() const {return fTime ;}
			inline time_t GetTimeGM() const {return mktime(std::gmtime(&fTime)) ;}

			std::string AsFormat(const std::string& frmt = "%d/%m/%Y %H:%M:%S"){
				char buffer[80];

				const char* format = frmt.c_str();
				strftime(buffer, 80, format, &fTM);

				return std::string(buffer);
			}

			static TimeDate FromFormat(const std::string& t, const std::string& fmt="%d/%m/%Y %H:%M:%S"){
				//put something
				TimeDate tm;
				const char* time_as_string = t.c_str();
				const char* format = fmt.c_str();
				strptime(time_as_string, format, &tm.fTM);
				tm.fTime = mktime(&tm.fTM);
				return tm;
			}

		};
	}
}


#endif //File Guardian