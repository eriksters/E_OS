#include "EOS_DataStructures.h"
#include "EOS_Core.h"
#include <stdio.h>

#define ARRAY_SIZE	20

static arrayList_t test_arrayList;
static arrayList_h test_arrayList_h = &test_arrayList;
static os_TCB_t * test_array[ARRAY_SIZE];

static os_TCB_t test_tcb_1;
static os_TCB_t test_tcb_2;
static os_TCB_t test_tcb_3;

void os_arrayList_test( void ){
	
	os_arrayList_init( test_arrayList_h, (void **) test_array, ARRAY_SIZE );
	
	os_arrayList_add( test_arrayList_h, &test_tcb_1 );
	os_arrayList_add( test_arrayList_h, &test_tcb_2 );
	os_arrayList_add( test_arrayList_h, &test_tcb_3 );
	
	os_arrayList_remove( test_arrayList_h, &test_tcb_2);
	
	__NOP();
} 

uint32_t os_arrayList_init( arrayList_h handle, void** array, uint32_t max_size ) {
	
	if ( max_size == 0 || array == 0 || handle == 0 ) {
		return 1;
	}
	
	handle->max_size = max_size;
	handle->array = array;
	
	return 0;
}


uint32_t os_arrayList_add( arrayList_h handle, void* E ) {
	
	//	Error if arrayList is full or element is NULL
	if ( handle->size == handle->max_size || E == 0 ) {
		return 1;
	}

	handle->array[handle->size] = E;
	
	handle->size++;
	
	return 0;
}


uint32_t os_arrayList_remove( arrayList_h handle, void* E ) {
	
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

uint32_t os_arrayList_contains( arrayList_h handle, void* ptr ) {

}

uint32_t os_arrayList_size( arrayList_h handle ) {

}