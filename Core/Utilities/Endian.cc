#include "Endian.hh"


bool ORCA::Utilities::isLittleEndian(){

	union probe
	{
		unsigned int num;
		unsigned char bytes[sizeof(unsigned int)];
	};
	probe p = { 1U }; //initialize first member of p with unsigned 1
	bool little_endian = (p.bytes[0] == 1U); //in a big endian architecture,
	//p.bytes[0] equals 0
	return little_endian;
}

bool ORCA::Utilities::isBigEndian(){
	return !isLittleEndian();
}
