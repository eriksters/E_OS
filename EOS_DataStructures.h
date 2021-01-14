#ifndef EOS_DATASTRUCTURES_H
#define EOS_DATASTRUCTURES_H

#include <stdint.h>


typedef struct {
	void ** array;
	uint32_t max_size;
	uint32_t size;
} os_arraylist_t;

typedef struct {
	void ** array;
	uint32_t head;
	uint32_t tail;
	uint32_t size;
	uint32_t max_size;
} os_queue_t;

typedef os_arraylist_t* os_arraylist_h;
typedef os_queue_t* os_queue_h;


/*******************************************************
*												Arraylist											 *
*******************************************************/


/*	Initialize an Arraylist
 *	Param handle: Pointer to Arraylist data structure
 *	Param array: Pointer to array where elements will be stored
 *	Param max_size: allocated size of array
 *	Returns handle if successful, NULL if handle is NULL or array is NULL or size is 0 
*/
os_arraylist_h os_arraylist_init( os_arraylist_t* arraylist_p, void** array, uint32_t max_size );


/*	Add an element to an Arraylist
 *	Param handle: handle for Arraylist
 *	Param E: element to add
 *  Returns 0 if successful, 1 if arraylist is full or element is NULL
*/
uint32_t os_arraylist_add( os_arraylist_h handle, void* E );


/*	Remove an element from an Arraylist
 *	Param handle: handle for Arraylist
 *	Param E: element to remove
 *  Returns 0 if successful, 1 if element is NULL or not found
*/
uint32_t os_arraylist_remove( os_arraylist_h handle, void* E );


/*	Get element by index
 *	Param handle: handle for Arraylist
 *	Param index: index of element to retrieve
 *  Returns element of given index, NULL if index out of bounds
*/
void* os_arraylist_get( os_arraylist_h handle, uint32_t index );


/*	Get size of Arraylist
 *	Param handle: handle for Arraylist
 *  Returns size of Arraylist
*/
uint32_t os_arraylist_size( os_arraylist_h handle );


/*	Check if Arraylist contains a given element
 *	Param handle: handle for Arraylist
 *	Param E: element to search for
 *  Returns 1 if element is found, 0 if not
*/
uint32_t os_arraylist_contains( os_arraylist_h handle, void* E );




/*******************************************************
*												Queue													 *
*******************************************************/



/*	Initialize a Queue
 *	Param handle: handle for Queue
 *	Param array: Pointer to array where elements will be stored
 *	Param max_size: allocated size of array
 *	Returns handle if successful, NULL if handle is NULL or array is NULL or size is 0 
*/
os_queue_h os_queue_init( os_queue_t* queue_p, void** array, uint32_t max_size );


/*	Add an element to Queue
 *	Param handle: handle for Queue
 *	Param E: element to add
 *  Returns 0 if successful, 1 if Queue is full or element is NULL
*/
uint32_t os_queue_add( os_queue_h handle, void* E );


/*	Remove and retrieve head element from Queue
 *	Param handle: handle for Queue
 *  Returns head element pointer if successful, NULL if queue is empty
*/
void* os_queue_remove( os_queue_h handle );


/*	Remove a specific element from Queue
 *	Param handle: handle for Queue
 *  Returns 0 if element was found, 1 if not
*/
uint32_t os_queue_remove_element( os_queue_h handle, void* E );


/*	Get amount of elements stored in Queue
 *	Param handle: handle for Queue
 *  Returns size of elements
*/
uint32_t os_queue_size( os_queue_h handle );


/*	Retrieve but not remove head element from Queue
 *	Param handle: handle for Queue
 *  Returns head element pointer
*/
void* os_queue_peek( os_queue_h handle );


/*	Check if Queue is full
 *	Param handle: handle for Queue
 *  Returns 1 if Queue is full, 0 if not
*/
uint32_t os_queue_is_full( os_queue_h handle );


/*	Check if Queue contains an element
 *	Param handle: handle for Queue
 *	Param E: element to search for
 *  Returns 1 if Queue contains given element, 0 if not
*/
uint32_t os_queue_contains( os_queue_h handle, void* E );
	
#endif
