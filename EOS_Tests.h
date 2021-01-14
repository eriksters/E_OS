#ifndef EOS_TESTS_H
#define EOS_TESTS_H

#include <stdint.h>
#include "EOS_DataStructures.h"


/*	Test the data structures module
 *	Returns the amount of failed tests is the data structures module
*/
uint32_t os_data_structures_test( void );


/*	Test all modules
 *	Returns amount of failed modules
*/
uint32_t os_all_tests( void );


/*	Unit test for the queue
 *	Param handle: handle for Queue
 *	Param array: Pointer to array where elements will be stored
 *	Param max_size: allocated size of array
 *	Returns number 0 if all tests passed, 1 if not
*/
uint32_t os_queue_test( void );


/*	Unit test for the arraylist
 *	Param handle: Pointer to Arraylist data structure
 *	Param array: Pointer to array where elements will be stored
 *	Param max_size: allocated size of array
 *	Returns number 0 if all tests passed, 1 if not
*/
uint32_t os_arraylist_test( void );

#endif
