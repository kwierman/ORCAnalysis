#include "Protium/Math/Random.hh"
#include "Protium/Utilities/SignalCatching.hh"

#include <iostream>
#include <fstream>
#include <iomanip>

int main(int argc, char* argv[]){

	//Setup the debug utilities, this may need to be done with other objects.
	Protium::Utilities::SignalCatcher::Instance().Setup();

	std::cout<<"Calculating Pi"<<std::endl;

	Protium::Math::Random rand;

	std::cout<<"\tWith Seed: "<<rand.getSeed()<<std::endl;

	std::ofstream outputFile;

	outputFile.open("random.out");


	//now, we're going to use this to 
	{
		double realPi = 3.14159265359;
		double error = 1.e-6;

		//iterate over the number of times I want to run this simulation
		for(int i=0; i<10000; i++){
			double nSquare=0;
			double nCircle =0;
			double radius = (double)i;			
			int iter=0;
			while(true){
				iter++;
				double x = rand.nextDouble(0.,radius);
				double y = rand.nextDouble(0,radius);
				if( (x*x+y*y<=radius*radius) )
					nSquare+=1;
				nCircle+=1;
				double calcPi = 4.0*nSquare/nCircle;
				if(calcPi<= realPi+error && calcPi>realPi-error){
					outputFile<<std::setprecision(15)<<iter<<","<<calcPi<<","<<radius<<std::endl;
					break;
				}
				if(iter>10000000){
					break;
				}
			}
		}
	}

	outputFile.close();

}