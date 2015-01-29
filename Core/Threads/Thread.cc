#include "Protium/Threads/Thread.hh"

void* Protium::Threads::Private::threadingFunction(void* thread){
	Thread* t = static_cast<Thread*>(thread);
	t->start();
	return NULL;
}

Protium::Threads::RootThread* Protium::Threads::RootThread::fRoot = NULL;