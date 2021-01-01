#ifndef EOS_DATASTRUCTURES_H
#define EOS_DATASTRUCTURES_H

#include <stdint.h>


typedef struct {
	void ** array;
	uint32_t max_size;
	uint32_t size;
} arrayList_t;

typedef arrayList_t* arrayList_h;

void os_arrayList_test( void );

uint32_t os_arrayList_init( arrayList_h handle, void** array, uint32_t max_size );

uint32_t os_arrayList_add( arrayList_h handle, void * E );

uint32_t os_arrayList_remove( arrayList_h handle, void* E );

uint32_t os_arrayList_contains( arrayList_h handle, void* E );

uint32_t os_arrayList_size( arrayList_h handle );

#endif
