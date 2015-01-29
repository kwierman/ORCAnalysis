#include "Protium/Singleton/DeletionPolicies.hh"


#include <assert.h>

#include <iostream>

//! A function that 
void Protium::Singleton::AtExitFn(){
	//TODO::Debug
	//std::cout<<"In Exit Function with list size: "<<fTrackerList->size()<<std::endl;
    assert( fTrackerList!=0 && !fTrackerList->empty());
    
    // Pick the element at the top of the stack
    DeletionTracker* pTop = fTrackerList->back();
    
    // Remove that object off the stack _before_ deleting pTop
    fTrackerList->pop_back();

    if(fTrackerList->empty() ){
    	delete fTrackerList;
    	fTrackerList=0;
    }
    
    // Destroy the element
    delete pTop;
}