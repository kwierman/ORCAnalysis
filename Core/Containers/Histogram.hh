#ifndef Protium_Histogram_hh_
#define Protium_Histogram_hh_

#include "Protium/Containers/Functional.hh"
#include "Protium/Allocation/SmallObject.hh"

#include <map>

namespace Protium{

	namespace Containers{

		/**
			\struct Bin
			\brief Represents a single bin with edge and width  
			\warning This may move to the interior of Histograms
		**/
		template<typename BinType>
		struct Bin : Protium::Allocation::DefaultSmallObject {
			BinType loweredge;
			BinType width;

			//! Copy constructor
			Bin(const Bin& other) : loweredge(other.loweredge) , width(other.width) {}

			//! Default constructor
			Bin(double lower=0, double w=0) : loweredge(lower), width(w) {}

			//! Assign operator copies contents
			Bin& operator=(const Bin& other){
				this->loweredge = other.loweredge;
				this->width = other.width;
				return *this;
			}

			//! if the bin and width fall below this bin and width
			bool operator<(const Bin& rhs) const{
				return ( (this->loweredge) < (rhs.loweredge) );
			}

			//! if the bin and width fall above this bin and width
			bool operator>(const Bin& rhs) const{
				return ( (this->loweredge+this->width) > (rhs.loweredge+rhs.width) );
			}

			//! if the bins overlap AT ALL
			bool operator==(const Bin& rhs) const{
				return ( (this->loweredge) == (rhs.loweredge) && this->width == rhs.width );
			}

			//! If the value falls below bin and width
			bool operator<(const BinType& rhs) const{
				return ( (this->loweredge) < rhs );
			}

			//! if the value falls above this bin
			bool operator>(const BinType& rhs) const{
				return (this->loweredge + this->width) > (rhs) ;
			}

			//! if the value falls within this  bin
			bool operator==(const BinType& rhs) const{
				return ((rhs>this->lowerEdge) && (rhs< this->lowerEdge+this->width) ) ;
			}

		};

		typedef Bin<double> DBin;
		typedef Bin<int> IBin;
		typedef Bin<unsigned> UBin;
		typedef Bin<float> FBin;


		/*
			\page Histogramming

			\section 1D Histogramming
				\note TODO: Rebinning needs to be coded in.
			\section 2D Histogramming
				\note TODO: This section is not complete
		*/

		template<class BinType=double>
		class Histogram1D : public Functional1D< Bin<BinType> , unsigned > {

			typedef Bin<BinType>							ThisBin;
			typedef Functional1D< Bin<BinType>, unsigned > HistType;
			typedef std::map< Bin<BinType>, unsigned >		DataType;
			typedef typename DataType::iterator DataIt;

		public:
			//! Most Basic Histogram instantiation. Creates uniform size bins
			//! \param lowerEdge Specifies the lowest value that this histogram should accept
			//! \param upperEdge Specifies the highest value that this histogram should accept.
			//! \param nBins Specifies the number of bins that this histogram should have
			//! \note The number of bins this histogram creates is nBins+2. The extra bins are for underflow/overflow. These can be probed by looking for values that are below the lowest edge or above the highest edge.
			Histogram1D( const BinType& lowerEdge, const BinType& upperEdge, const unsigned& nBins){
				double width = (upperEdge-lowerEdge)/double(nBins);
				for(unsigned i=0; i<nBins; i++){
					Bin<BinType> tempBin;
					tempBin.width = width;
					tempBin.loweredge = lowerEdge+double(i)*width;
					this->fData.insert(std::make_pair< Bin<BinType>, unsigned>(tempBin,0) );
				}
			}


			//! Returns the number of bins in this histogram
			unsigned GetNBins() const{
				return HistType::fData.size();
			}

			//! Use this for Rebinning. 
			//! Bins overlapping this one get resized so that:
			//!    The bin overlapping the lower edge gets rebinned to the lower edge
			//!    The bin overlapping the upper edge gets rebinned to the upper edge
			//!    The bins overlapping the middle range (inclduing those with matching edges of the new bin)
			//!    get deleted and replaced by the new bin (bin content gets transferred).
			/*void SetNewBin(const BinType& lowerEdge, const BinType& delta ){
				Bin newBin;
				newBin.width=delta;
				newBin.loweredge = lowerEdge;
				unsigned newContent =0;
				typename std::map<Bin,unsigned>::iterator hIt = fContents.begin();
				//move iterator to the first bin intersects with the  
				while( hIt->first < newBin ){hIt++;}


				fContents[newBin] = newContent;
			}
			*/
			//void AddBin(const BinType& edge, const BinType& width);

			BinType GetLowerEdgeByIndex(const unsigned& i) const {


				for(unsigned int j =0; j<i; j++){

				}


				return BinType(1e6);
			}

			//! Get the bin width for a given index
			BinType GetBinWidthByIndex(const int& i) const {
				return HistType::fData[i].first.width;	
			}

			//! Get the bin lower edge for a given value
			BinType GetLowerEdgeByValue(const BinType& i) const {

				return (*HistType::fData.lower_bound(i)).first.loweredge;
			}

			//! Get a bin width for a given value
			BinType GetBinWidthByValue(const int& i) const {
				return (*HistType::fData.lower_bound(i)).first.width;	
			}


			//! Iterates a bin content by a weight (default 1)
			void FillByIndex(const unsigned& index, const unsigned& weight=1){
				HistType::fData[index].second+=weight;	
			}

			//! Iterates a bin content by a weight (default 1)
			void FillByValue(const BinType& val, const unsigned& weight=1){
				//replace this with a 
				ThisBin temp(val);
				HistType::RefAt(temp)+= weight;
			}

			//! Find the bin content by index in the map
			unsigned GetByIndex(const unsigned& i){
				return HistType::fData[i].second;	
			}

			//! Find teh bin content by value
			unsigned GetByValue(const BinType& val){
				ThisBin temp(val);
				return HistType::RefAt(temp );
			}
		};

		//! \class Hist1D
		//! \brief A dimensional double float precision histogram
		typedef Histogram1D<double> Hist1D;
	}
}


#endif //Protium_Histogram_h_