#include "Protium/Threads/Mutex.hh"

Protium::Threads::Mutex::Mutex() : fIsLocked(false){
    pthread_mutex_init(&fMtx, 0);
}

Protium::Threads::Mutex::~Mutex(){
	pthread_mutex_destroy(&fMtx);
}

void Protium::Threads::Mutex::Lock(){
    pthread_mutex_lock(&fMtx); 
    fIsLocked = true;
}

void Protium::Threads::Mutex::Unlock(){
    pthread_mutex_unlock(&fMtx); 
    fIsLocked = false;
}

bool Protium::Threads::Mutex::IsLocked(){
    return fIsLocked;
}