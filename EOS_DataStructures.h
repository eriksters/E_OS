#ifndef EOS_DATASTRUCTURES_H
#define EOS_DATASTRUCTURES_H

#include <stdint.h>


typedef struct {
	void ** array;
	uint32_t max_size;
	uint32_t size;
} arrayList_t;

typedef arrayList_t* arrayList_h;


/*	Initialize an ArrayList
 *	Param handle: handle for ArrayList
 *	Param array: Pointer to array where elements will be stored
 *	Param max_size: allocated size of array
 *	Returns 0 if successful, 1 if handle is NULL or array is NULL or size is 0 
*/
uint32_t os_arrayList_init( arrayList_h handle, void** array, uint32_t max_size );


/*	Add an element to an ArrayList
 *	Param handle: handle for ArrayList
 *	Param E: element to add
 *  Returns 0 if successful, 1 if arrayList is full or element is NULL
*/
uint32_t os_arrayList_add( arrayList_h handle, void * E );


/*	Remove an element from an ArrayList
 *	Param handle: handle for ArrayList
 *	Param E: element to remove
 *  Returns 0 if successful, 1 if element is NULL or not found
*/
uint32_t os_arrayList_remove( arrayList_h handle, void* E );


/*	Check if ArrayList contains a given element
 *	Param handle: handle for ArrayList
 *	Param E: element to search for
 *  Returns 1 if element is found, 0 if not
*/
uint32_t os_arrayList_contains( arrayList_h handle, void* E );



#endif
