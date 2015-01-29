
#include "Protium/Threads/Thread.hh"

#include <iostream>
/*
class TerminatorThread : Protium::Threads::Thread{

public:
	virtual void start(){
		for(int i=0; i<3;i++){
			//std::cout<<"Now in thread: "<<this->getPID()<<std::endl;	
		}
	}
};


class ChildThread : Protium::Threads::Thread{
	//maybe replace this with something that 
public:
	virtual void start(){
		for(int i=0; i<3;i++){
			//std::cout<<"Now in thread: "<<this->getPID()<<" starting child thread..."<<std::endl;
			this->CreateChild<TerminatorThread>()->Start();
		}
		std::cout<<"Waiting for child threads to finish out..."<<std::endl;
		for(childthreadit it = getChildren().begin(); it!= getChildren().end(); ++it){
			Status stat = this->join(*it);
			if(stat != exited){
				std::cout<<"Warning: Thread did not exit correctly: "
			}
		}
	}
};
*/

int main(int argc, char* argv[]){
	//Protium::Threads::RootThread::GetRootThread();

/*	//create the direct child thread and let it go
	for(int i=0; i< 3; i++)
		Protium::Threads::RootThread::GetInstance().CreateChild<ChildThread>()->Start();
	//now wait for all the threads to join up. Maybe move this to the parent process.
	Protium::Threads::RootThread root = Protium::Threads::RootThread::GetInstance();
	for(ChildThreadIt it = root.getChildren().begin() ; it!= root.getChildren.end(); ++it){
		Status stat = this->join(*it);
		if(stat!=exited){
			std::cout<<"Warning: parent thread could not kill child thread"<<std::endl;
		}
	} 
	*/
}