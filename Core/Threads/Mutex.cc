#include "ORCA/Threads/Mutex.hh"

ORCA::Threads::Mutex::Mutex() : fIsLocked(false){
    pthread_mutex_init(&fMtx, 0);
}

ORCA::Threads::Mutex::~Mutex(){
	pthread_mutex_destroy(&fMtx);
}

void ORCA::Threads::Mutex::Lock(){
    pthread_mutex_lock(&fMtx); 
    fIsLocked = true;
}

void ORCA::Threads::Mutex::Unlock(){
    pthread_mutex_unlock(&fMtx); 
    fIsLocked = false;
}

bool ORCA::Threads::Mutex::IsLocked(){
    return fIsLocked;
}