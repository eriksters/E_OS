#include "EOS_Mutex.h"


static os_mutex_t* os_mutex_array[OS_MAX_MUTEX_COUNT];
static os_arraylist_t os_mutex_arraylist;

static os_arraylist_h os_mutex_arraylist_H;


void os_mutex_init( void ) {
	//	Initialize Mutex Arraylist
	os_mutex_arraylist_H = os_arraylist_init( &os_mutex_arraylist, (void**) os_mutex_array, OS_MAX_MUTEX_COUNT );
}

os_mutex_h os_create_mutex_call_handler( os_mutex_t* mutex_p ) {
	
	//	Error if mutex is already initialized
	if ( os_arraylist_contains( os_mutex_arraylist_H, mutex_p ) ) {
		return 0;
	}
	
	//	Error if mutex can not be added to mutex list
	if (os_arraylist_add( os_mutex_arraylist_H, mutex_p )) {
		return 0;
	}
	
	mutex_p->owner = 0;
	
	return (os_mutex_h) mutex_p;
}


uint32_t os_lock_mutex_call_handler( os_mutex_h mutex ) {

	//	If Mutex is locked by a task, return 1
	if ( mutex->owner != 0 ) {
		return 1;
	}
	
	//	Assign Mutex's owner to calling task
	mutex->owner = os_get_current_task();
	
	return 0;

}

uint32_t os_unlock_mutex_call_handler( os_mutex_h mutex ) {
	
	//	If task is locked by another thread or not locked at all, return 1
	if ( mutex->owner != os_get_current_task() ) {
		return 1;
	}
	
	//	Unlock the thread
	mutex->owner = 0;
	
	return 0;
}

void os_unlock_all_mutexes_by_task( os_task_h task ) {
	
	uint32_t size = os_arraylist_size( os_mutex_arraylist_H );
	os_mutex_h mutex;
	
	for ( uint32_t i = 0; i < size; i++ ) {
		mutex = os_arraylist_get( os_mutex_arraylist_H, i );
		if ( mutex->owner == task ) {
			mutex->owner = 0;
		}
	}
}
