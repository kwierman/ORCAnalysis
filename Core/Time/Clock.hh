#ifndef Protium_Clock_hh_
#define Protium_Clock_hh_

#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <vector>

namespace Protium{
	namespace Time{

		/**
			Class for defining methods of retrieving machine clock cycles
		**/
		class ClockPrimitive{
			clock_t fClock;
		public:
			//! Constructor sets up clock to the current time.
			ClockPrimitive(){
				Set();
			}
			//! Sets the clock to now.
			inline void Set(){
				fClock = clock();
			}
			//! Getter for the internal structure
			//! \return the C-Style clock struct
			inline clock_t Get() const{
				return fClock;
			}
			//! Statically returns the normalization condition for the machine
			//! \note POSIX requires that this always return 1000000
			static clock_t CPS(){
				return CLOCKS_PER_SEC;
			}
			//! Copy Constructor copies the time interior
			ClockPrimitive(const ClockPrimitive& other) : fClock(other.fClock){}
			//! Assignment operator copies time interior
			ClockPrimitive& operator=(const ClockPrimitive& other) {
				this->fClock = other.fClock;
				return *this;
			}
			//! Forwards this to the time interior structs
			bool operator==(const ClockPrimitive& other){
				return (this->fClock == other.fClock );
			}

			//! Returns a time difference for clocking time differentials
			ClockPrimitive& operator-(const ClockPrimitive& other){
				this->fClock - other.fClock;
				return *this;				
			}

		};

		/**
			Defines functions for clocking certain times
		**/
		class SingleClocker{
			ClockPrimitive fStart;
			ClockPrimitive fStop;
		public:
			SingleClocker() {}
			inline void Start(){fStart.Set();}
			inline void Stop(){fStop.Set();}
			inline clock_t GetTime(){return fStop.Get() - fStart.Get();}
		};

		/**
			Gives stop-watch style functionality to clocking applications
		**/
		class RepeatClocker{
			ClockPrimitive fStart;
			//* Every Time we 
			std::vector<ClockPrimitive> fData;
		public:
			RepeatClocker(){}
			inline void Clock(){
				fData.push_back( ClockPrimitive() );
			}
			unsigned GetNClocks() const {
				return fData.size();
			}
			clock_t GetStart(){
				return fStart.Get();
			}
			clock_t Get(const unsigned& i=0)const {
				return fData[i].Get();
			}
			clock_t GetOffset(const unsigned& i=0)const {
				return fData[i].Get() - fStart.Get();
			}

		};
	}
}

#endif //File Guardian