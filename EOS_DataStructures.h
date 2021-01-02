#ifndef EOS_DATASTRUCTURES_H
#define EOS_DATASTRUCTURES_H

#include <stdint.h>


typedef struct {
	void ** array;
	uint32_t max_size;
	uint32_t size;
} os_arrayList_t;

typedef struct {
	void ** array;
	uint32_t head;
	uint32_t tail;
	uint32_t size;
	uint32_t max_size;
} os_queue_t;

typedef os_arrayList_t* os_arrayList_h;
typedef os_queue_t* os_queue_h;


/*	Initialize an ArrayList
 *	Param handle: handle for ArrayList
 *	Param array: Pointer to array where elements will be stored
 *	Param max_size: allocated size of array
 *	Returns 0 if successful, 1 if handle is NULL or array is NULL or size is 0 
*/
uint32_t os_arrayList_init( os_arrayList_h handle, void** array, uint32_t max_size );


/*	Add an element to an ArrayList
 *	Param handle: handle for ArrayList
 *	Param E: element to add
 *  Returns 0 if successful, 1 if arrayList is full or element is NULL
*/
uint32_t os_arrayList_add( os_arrayList_h handle, void * E );


/*	Remove an element from an ArrayList
 *	Param handle: handle for ArrayList
 *	Param E: element to remove
 *  Returns 0 if successful, 1 if element is NULL or not found
*/
uint32_t os_arrayList_remove( os_arrayList_h handle, void* E );


/*	Get element by index
 *	Param handle: handle for ArrayList
 *	Param index: index of element to retrieve
 *  Returns element of given index, NULL if index out of bounds
*/
void* os_arrayList_get( os_arrayList_h handle, uint32_t index );


/*	Get size of ArrayList
 *	Param handle: handle for ArrayList
 *  Returns size of ArrayList
*/
uint32_t os_arrayList_size( os_arrayList_h handle );


/*	Check if ArrayList contains a given element
 *	Param handle: handle for ArrayList
 *	Param E: element to search for
 *  Returns 1 if element is found, 0 if not
*/
uint32_t os_arrayList_contains( os_arrayList_h handle, void* E );


uint32_t os_queue_init( os_queue_h handle, void** array, uint32_t max_size );

uint32_t os_queue_add( os_queue_h handle, void * E );

uint32_t os_queue_remove( os_queue_h handle, void* E );

uint32_t os_queue_size( os_queue_h handle );

uint32_t os_queue_peek( os_queue_h handle, void* E );
	
#endif
