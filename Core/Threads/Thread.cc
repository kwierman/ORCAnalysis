#include "ORCA/Threads/Thread.hh"

void* ORCA::Threads::Private::threadingFunction(void* thread){
	Thread* t = static_cast<Thread*>(thread);
	t->start();
	return NULL;
}

ORCA::Threads::RootThread* ORCA::Threads::RootThread::fRoot = NULL;