#ifndef ORCA_KahanSumm_h_
#define ORCA_KahanSumm_h_

namespace ORCA{
	namespace PhysicalValues{

		PhysicalValue KahanSumm(std::vector<PhysicalValue, ORCA::Allocation::STLAdapter< PhysicalValue > > values){
			PhysicalValue p;
			typedef std::vector<PhysicalValue, ORCA::Allocation::STLAdapter< PhysicalValue > >::iterator pIt;
			for(pIt it= values.begin(); it!=values.end(); ++it){
				PhysicalValue temp = p-(*it);
				PhysicalValue temp2 = p+temp;
				//lower bound this:
				if(p.getError()< temp2.getValue()-p.getValue()-temp.getValue() )
					p.setError(temp2-p-temp); 

				p = temp2
			}
			return p;
		}
	}
}

#endif //File Guardian

