#include "EOS_DataStructures.h"
#include "EOS_Core.h"
#include <stdio.h>

/*
#define ARRAY_SIZE	20

static os_arrayList_t test_arrayList;
static os_arrayList_h test_os_arrayList_h = &test_arrayList;
static os_TCB_t * test_array[ARRAY_SIZE];

static os_TCB_t test_tcb_1;
static os_TCB_t test_tcb_2;
static os_TCB_t test_tcb_3;



void os_os_arrayList_test( void ){
	
	uint32_t contains = 0;
	
	os_arrayList_init( test_os_arrayList_h, (void **) test_array, ARRAY_SIZE );
	
	os_arrayList_add( test_os_arrayList_h, &test_tcb_1 );
	os_arrayList_add( test_os_arrayList_h, &test_tcb_2 );
	os_arrayList_add( test_os_arrayList_h, &test_tcb_3 );
	
	contains = os_arrayList_contains( test_os_arrayList_h, &test_tcb_2 );
	
	os_arrayList_remove( test_os_arrayList_h, &test_tcb_2);
	
	contains = os_arrayList_contains( test_os_arrayList_h, &test_tcb_2 );
	
	__NOP();
} 
*/

uint32_t os_arrayList_init( os_arrayList_h handle, void** array, uint32_t max_size ) {
	
	//	Error if handle is NULL or array is NULL or size is 0 
	if ( max_size == 0 || array == 0 || handle == 0 ) {
		return 1;
	}
	
	handle->max_size = max_size;
	handle->array = array;
	
	return 0;
}


uint32_t os_arrayList_add( os_arrayList_h handle, void* E ) {
	
	//	Error if arrayList is full or element is NULL
	if ( handle->size == handle->max_size || E == 0 ) {
		return 1;
	}
	
	
	handle->array[handle->size] = E;
	
	handle->size++;
	
	return 0;
}


uint32_t os_arrayList_remove( os_arrayList_h handle, void* E ) {
	
	uint32_t found = 0;
	uint32_t index = 0;
	
	
	//	Error if element is NULL
	if ( E == 0 ) {
		return 1;
	}
	
	
	//	Find element index
	for ( uint32_t i = 0; i < handle->size; i++ ) {
		if ( handle->array[i] == E ) {
			found = 1;
			index = i;
			break;
		}
	}
	
	
	//	Error if element not found
	if ( !found ) {
		return 1;
	}

	
	//	Shift all other elements 1 position left
	for ( uint32_t i = index; i != (handle->size - 1) ; i++ ) {
		handle->array[i] = handle->array[i + 1];
	}
	
	handle->size--;
	
	return 0;
}


void* os_arrayList_get( os_arrayList_h handle, uint32_t index ) {
	
	//	Error if index is out of bounds
	if ( index >= handle->max_size ) {
		return 0;
	}
	
	return handle->array[index];
}

uint32_t os_arrayList_size( os_arrayList_h handle ) {
	return handle->size;
}

uint32_t os_arrayList_contains( os_arrayList_h handle, void* E ) {
	
	uint32_t found = 0;
	
	//	Error if element is NULL
	if ( E == 0 ) {
		return 0;
	}
	
	//	Find element
	for ( uint32_t i = 0; i < handle->size; i++ ) {
		if ( handle->array[i] == E ) {
			found = 1;
			break;
		}
	}

	return found;
}
//

/*
#define TEST_ARRAY_SIZE		10
static os_queue_t test_queue;
static os_queue_h test_queue_h = &test_queue;
static uint32_t* test_array[TEST_ARRAY_SIZE];

void os_queue_test( void ) {
	
	uint32_t* b = 0;
	
	os_queue_init( test_queue_h, (void**) test_array, TEST_ARRAY_SIZE );
	
	os_queue_add( test_queue_h, (uint32_t*) 0x10 );
	os_queue_add( test_queue_h, (uint32_t*) 0x20 );
	os_queue_add( test_queue_h, (uint32_t*) 0x30 );
	os_queue_add( test_queue_h, (uint32_t*) 0x40 );
	
	b = os_queue_remove( test_queue_h );
	os_queue_remove( test_queue_h );
	os_queue_remove( test_queue_h );
	b = os_queue_remove( test_queue_h );
	b = os_queue_peek( test_queue_h );
	b = os_queue_remove( test_queue_h );
	
}
*/

uint32_t os_queue_init( os_queue_h handle, void** array, uint32_t max_size ) {
	
	//	Error if handle is NULL or array is NULL or size is less than 1
	if ( handle == 0 || array == 0 || max_size < 1 ) {
		return 1;
	}
	
	handle->array = array;
	handle->max_size = max_size;
	handle->size = 0;
	handle->head = 0;
	handle->tail = max_size - 1;
	
	return 0;
}

uint32_t os_queue_add( os_queue_h handle, void * E ) {
	
	//	Error if E is NULL or queue is full
	if ( handle->size == handle->max_size || E == 0 ) {
		return 1;
	}
	
	handle->tail++;
	
	//	Check for wrap-around
	if ( handle->tail == handle->max_size ) {
		handle->tail = 0;
	}
	
	handle->array[handle->tail] = E;
	handle->size++;
	
	return 0;
}

void* os_queue_remove( os_queue_h handle ) {
	
	void* ret = 0;
	
	//	Return NULL if no elements in queue
	if ( handle->size == 0 ) {
		return 0;
	}
	
	//	Get head element
	ret = handle->array[handle->head];
	
	
	handle->head++;
	
	//	Wrap-around
	if ( handle->head == handle->max_size ) {
		handle->head = 0;
	}
	
	handle->size--;
	
	return ret;
}

uint32_t os_queue_size( os_queue_h handle ) {
	return handle->size;
}

void* os_queue_peek( os_queue_h handle ) {
	return handle->array[handle->head];
}

uint32_t os_queue_is_full( os_queue_h handle ) {
	return ( handle->size == handle->max_size );
}
