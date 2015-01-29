#include "Protium/Math/Random.hh"

#include <ctime>
#include <cassert>

Protium::Math::Random::Random(unsigned seed):fSeed(seed?seed : static_cast<unsigned>( std::time(0) ) ) {}

void Protium::Math::Random::setSeed(unsigned seed){
	fSeed = (seed? seed : static_cast<unsigned>(std::time(0)));
}
unsigned Protium::Math::Random::getSeed(){return fSeed;}


unsigned short Protium::Math::Random::nextShort(){

	///TODO: There is a warning generated here.
    static const unsigned a =24699;
    return static_cast<unsigned short>(fSeed =a*(fSeed & 65535)+ (fSeed >> 16) );
}



unsigned int Protium::Math::Random::nextUint()
{
    return (unsigned int)nextShort() << (16 * sizeof(unsigned short)) |
        nextShort();
}
unsigned int Protium::Math::Random::nextUint(unsigned int high)
{
    assert(high < 4294967295 - 1);
    ++high;
    const unsigned int bucket_size = 4294967295 / high;
    unsigned int a;
    do 
    {
        a = nextUint() / bucket_size;
    }
    while (a >= high);
    return a;
}

double Protium::Math::Random::nextDouble(double low, double high){
    //highest short: 65535
    double retValue = static_cast<double>(nextShort() ) - low;
    retValue*=(high-low)/(65535.-0.);
    return retValue;
}
