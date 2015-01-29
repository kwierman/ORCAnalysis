/*! \file protium_init_tets.cxx
	\brief Home-brew unit tests. To be replaces by google tests in the future.
**/

#include "Protium/Utilities/SignalCatching.h"
#include "Protium/LinearAlgebra/Matrix.h"
#include "Protium/Containers/Histogram.h"
#include "Protium/Math/MathFunctions.h"
#include "Protium/Time/Time.h"
#include "Protium/Time/Clock.h"

#include <iostream>
#include <iomanip>
#include <vector>

using namespace Protium;

template<typename T, int n>
void PrintVector(const LinearAlgebra::Vector<T,n>& vector){
	std::cout<<"Vector of Length: "<<n<<std::endl;
	for(int i=0; i<n;i++)std::cout<<" "<<vector.At(i);
	std::cout<<std::endl;
}

template<typename T, int n, int m>
void PrintMatrix(const LinearAlgebra::Matrix<T,n,m>& matrix){
	std::cout<<"Matrix of Dimensions: "<<n<<" X "<<m<<std::endl;
	for(int i=0; i<n;i++){
		for(int j=0; j<m;j++)
			std::cout<<" "<<matrix.At(i,j);
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
}


void PrintHistogram(const Containers::Histo1D& h){
	unsigned nBins = h.GetNBins();
	std::cout<<"Getting Histogram of "<<nBins<<" bins"<<std::endl;
	for(unsigned i=0;i<nBins;i++){
		std::cout<<"  Bin Number: "<<i<<" Lower Edge: "<<h.GetLowerEdgeByI(i)<<" Width: "<<h.GetBinWidthByI(i)<<" Content: "<<h.GetBinContentByI(i)<<std::endl;
	}

}


int main(int argc, char* argv[]){

	Protium::Utilities::SignalCatcher::Instance().Setup();

	std::cout<<"Starting Basic Unit Tests"<<std::endl;
	std::cout<<"\tMath tests"<<std::setprecision(16)<<std::endl;
	{
		using namespace Math;
		std::cout<<"\t\tInt factorial (6): "<< 
			factorial<unsigned>(3)<<"\t"<<factorial<int>(3)<<
			"\t"<<factorial<long>(3)<<std::endl;

		std::cout<<"\t\tFloat/Double factorial (6): "<< 
			factorial<float>(3)<<"\t"<<factorial<double>(3)<<
			"\t"<<factorial<long double>(3)<<std::endl;

		std::cout<<"\t\tEuler\'s Number (2.71828182845904523536028747135266249775724709369995): "<<nat<<std::endl;
		std::cout<<"\t\t\tError: "<<2.71828182845904523536028747135266249775724709369995-nat<<std::endl;
		std::cout<<"\t\tPowers: "<<std::endl;
		for(double i=0; i<15;i++)for(double j=0; j<15; j++)
			std::cout<<"\t"<<power(i,j)<<"\t";
		std::cout<<std::endl;
	}
	
	std::cout<<"\tLinear Algebra Tests"<<std::endl;
	{
		using namespace LinearAlgebra;
		std::cout<<"Testing Vectors"<<std::endl;
		{
			TwoVector vec;
			PrintVector(vec);
			PrintVector(TwoVector::UnitVector());
			PrintVector(vec.GetSubVector(1));
		}
		{
			ThreeVector vec;
			PrintVector(vec);
			PrintVector(ThreeVector::UnitVector());
			PrintVector(vec.GetSubVector(1));
			PrintVector(vec.GetSubVector(1).GetSubVector(1));
		}
		{
			FourVector vec;
			PrintVector(vec);
			PrintVector(FourVector::UnitVector());
			PrintVector(vec.GetSubVector(1));
			PrintVector(vec.GetSubVector(1).GetSubVector(1));
			PrintVector(vec.GetSubVector(1).GetSubVector(1).GetSubVector(1));
		}
		{
			FiveVector vec;
			PrintVector(vec);
			PrintVector(FiveVector::UnitVector());
			std::cout<< (vec*FiveVector::UnitVector()) <<std::endl;
			PrintVector(vec+FiveVector::UnitVector());
			PrintVector(vec-FiveVector::UnitVector());
		}
		{
			SixVector vec;
			PrintVector(vec);
			PrintVector(SixVector::UnitVector());
		}
		{
			SevenVector vec;
			PrintVector(vec);
			PrintVector(SevenVector::UnitVector());
		}
		{
			EightVector vec;
			PrintVector(vec);
			PrintVector(EightVector::UnitVector());
		}
		{
			NineVector vec;
			PrintVector(vec);
			PrintVector(NineVector::UnitVector());
		}
		{
			TenVector vec;
			PrintVector(vec);
			PrintVector(TenVector::UnitVector());
		}
		{
			ElevenVector vec;
			PrintVector(vec);
			PrintVector(ElevenVector::UnitVector());
		}
		{
			TwelveVector vec;
			PrintVector(vec);
			PrintVector(TwelveVector::UnitVector());
		}
		{
			ThirteenVector vec;
			PrintVector(vec);
			PrintVector(ThirteenVector::UnitVector());
		}
		{
			FourteenVector vec;
			PrintVector(vec);
			PrintVector(FourteenVector::UnitVector());
		}
		{
			FifteenVector vec;
			PrintVector(vec);
			PrintVector(FifteenVector::UnitVector());
			std::cout<< (vec==FifteenVector::UnitVector() )<<std::endl;
			std::cout<< (vec!=FifteenVector::UnitVector() )<<std::endl;
			std::cout<< (FifteenVector::UnitVector()==FifteenVector::UnitVector() )<<std::endl;
			FifteenVector copy(FifteenVector::UnitVector() );
			PrintVector(copy);
			copy=vec;
			PrintVector(copy);
		}
		std::cout<<"Testing Matrices"<<std::endl;
		{
			TwoMatrix mat;
			PrintMatrix(mat);
			PrintMatrix(TwoMatrix::Unit());
		}
		{
			ThreeMatrix mat;
			PrintMatrix(mat);
			PrintMatrix(ThreeMatrix::Unit());
		}
		{
			FourMatrix mat;
			PrintMatrix(mat);
			PrintMatrix(FourMatrix::Unit());
		}
		{
			FiveMatrix mat;
			PrintMatrix(mat);
			PrintMatrix(FiveMatrix::Unit());
		}
		{
			SixMatrix mat;
			PrintMatrix(mat);
			PrintMatrix(SixMatrix::Unit());
		}
		{
			SevenMatrix mat;
			PrintMatrix(mat);
			PrintMatrix(SevenMatrix::Unit());
			SevenMatrix copy(SevenMatrix::Unit() );
			PrintMatrix(copy);
			copy=mat;
			PrintMatrix(copy);
			copy=SevenMatrix::Unit();
			for(int i=0; i<7;i++)for(int j=0; j<7;j++) copy[i][j]=1+(i-j)*(i-j)*(i-j)*(i-j)*(i-j);
			PrintMatrix( copy );
			PrintMatrix(copy.GetSubMatrix(0,0) );
			PrintMatrix(copy.GetSubMatrix(0,0).GetSubMatrix(0,0) );
			PrintMatrix(copy.GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0) );
			PrintMatrix(copy.GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0) );
			PrintMatrix(copy.GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0) );
			PrintMatrix(copy.GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0) );

			PrintMatrix( copy );
			std::cout<<copy.GetSubMatrix(0,0).GetDeterminant()<<std::endl;
			std::cout<<copy.GetSubMatrix(0,0).GetSubMatrix(0,0).GetDeterminant()<<std::endl;
			std::cout<<copy.GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetDeterminant()<<std::endl;
			std::cout<<copy.GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetDeterminant()<<std::endl;
			std::cout<<copy.GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetDeterminant()<<std::endl;
			std::cout<<copy.GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetSubMatrix(0,0).GetDeterminant()<<std::endl;
		}

		{
			std::cout<<"Vector Times Matrix Multiplication"<<std::endl;
			SevenVector vec;
			for(int i=0; i<7;i++)vec[i]=i;
			SevenMatrix mat = SevenMatrix::Unit();
			for(int i=0; i<7;i++)mat[0][i]=i;
			PrintVector(mat*vec );
		}
	}
	std::cout<<"Testing Histogram Classes"<<std::endl;
	{
		using namespace Containers;
		{
			Histo1D h(0.0, 100.0, 1000);
			PrintHistogram(h);
			h.SetNewBin(98.75, 1.0);
		}
	}
	std::cout<<"Testing Time Functions"<<std::endl;
	{
		using namespace Time;
		
		SingleClocker clk;
		std::cout<<"Testing Single Clock at: "<<ClockPrimitive::CPS()<<" CPS"<<std::endl;
		clk.Stop();
		std::cout<<"Single Clock Output"<<clk.GetTime()<<std::endl;
		clk.Stop();
		std::cout<<"Single Clock Output"<<clk.GetTime()<<std::endl;
		clk.Stop();
		std::cout<<"Single Clock Output"<<clk.GetTime()<<std::endl;
		RepeatClocker rpt;
		for(int i=0; i<100;i++)
			rpt.Clock();
		std::cout<<"Testing RepeatClocker: Start Time: "<<rpt.GetStart();
		for(int i=0; i<rpt.GetNClocks();i++)
			std::cout<<" Clock: "<<rpt.Get(i)<<" Offset: "<<rpt.GetOffset(i)<<std::endl;

		std::cout<<"Testing Time..."<<std::endl;
		TimePrimitive prim = TimePrimitive::Now();
		TimeDate t(prim);
		std::cout<<"The Time now is: "<<t.AsFormat()<<std::endl;
		TimeDate other = TimeDate::FromFormat("1/4/1988 19:30:15");
		std::cout<<"Other Time is: "<<other.AsFormat()<<std::endl;

	}
}

