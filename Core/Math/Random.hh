#ifndef Protium_Random_hh_
#define Protium_Random_hh_



namespace Protium{
    namespace Math{
        /**
         *  \class Random
         *  \author Kevin Wierman
         *  \brief Random Number generator utilizing multiply-with-carry.
         *
         *  Multiply-with-carry was invented by George Marsaglia. This utilizes a similar method
         *  wherein the resultant is taken as the last 16-bits, but the full result is propagated 
         *  to the seed.
        **/
        class Random{
            unsigned fSeed;
        public:
            Random(unsigned seed=0);
            void setSeed(unsigned seed=0);
            unsigned getSeed();
            unsigned short nextShort();            
            unsigned int nextUint();
            unsigned int nextUint(unsigned int high);
            double nextDouble(double low, double high);
        };
    }
}



#endif //File Guardian


