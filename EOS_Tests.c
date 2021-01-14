#include "EOS_Tests.h"
#include "EOS_DataStructures.h"

#define DATA_STRUCTURE_SIZE 	5

uint32_t os_all_tests( void ) {
	
	uint32_t failed_modules = 0;
	
	if ( os_data_structures_test() > 0 ) {
		failed_modules++;
	}
	
	return failed_modules;
}


uint32_t os_data_structures_test( void ) {
	
	uint32_t failed_tests = 0;
	
	if ( os_arraylist_test() != 0 )
		failed_tests++;
	
	if ( os_queue_test() != 0 )
		failed_tests++;
	
	return failed_tests;
	
}


uint32_t os_queue_test( void ) {
	
	uint32_t* test_item = (uint32_t*) 0xF0F0F0F0;
	
	uint32_t* test_array[DATA_STRUCTURE_SIZE];
	os_queue_t test_queue;
	
	//	Init
	os_queue_h queue = os_queue_init( &test_queue, (void**) test_array, DATA_STRUCTURE_SIZE );
	
	if ( queue == 0 ) 
		return 1; 
	
	
	//	Test add 
	if ( os_queue_add( queue, test_item ) != 0 ) 
		return 1;
	
	
	//	Test size
	if ( os_queue_size( queue ) != 1 ) 
		return 1;
	
	
	//	Test contains
	if ( !os_queue_contains( queue, test_item )) 
		return 1;
	
	
	//	Test peek
	if ( os_queue_peek( queue ) != test_item ) 
		return 1;
	
	
	//	Test remove
	if ( os_queue_remove( queue ) != test_item ) 
		return 1;
	
	
	//	Test get sequence / remove element
	os_queue_add( queue, test_item );
	os_queue_add( queue, test_item + 1 );
	
	if ( os_queue_peek( queue ) != test_item )
		return 1;
	
	if ( os_queue_remove_element( queue, test_item ) != 0 ) 
		return 1;
	
	if ( os_queue_contains( queue, test_item ))
		return 1;
	
	if ( !os_queue_contains( queue, test_item + 1 )) 
		return 1;
	
	if ( os_queue_remove_element( queue, test_item + 1 ) != 0 )
		return 1;
	
	
	//	Test size limit and wrap-around
	for ( uint32_t i = 1; i < DATA_STRUCTURE_SIZE + 1; i++ ) {
		if ( os_queue_add( queue, (uint32_t*) i ) != 0 ) 
			return 1;
	}
	
	if ( os_queue_add( queue, (uint32_t*) test_item ) == 0 )
		return 1;
	
	
	//	Test is full
	if ( !os_queue_is_full( queue ) )
		return 0;
	
	return 0;
	
}


uint32_t os_arraylist_test( void ){
	
	uint32_t* test_item = (uint32_t*) 0xF0F0F0F0;
	
	uint32_t* test_array[DATA_STRUCTURE_SIZE];
	os_arraylist_t test_arraylist;
	
	//	Init
	os_arraylist_h list = os_arraylist_init( &test_arraylist, (void**) test_array, DATA_STRUCTURE_SIZE );
	
	if ( list == 0 ) 
		return 1; 
	
	
	//	Test add 
	if ( os_arraylist_add( list, test_item ) != 0 ) 
		return 1;
	
	
	//	Test size
	if ( os_arraylist_size( list ) != 1 ) 
		return 1;
	
	
	//	Test contains
	if ( !os_arraylist_contains( list, test_item )) 
		return 1;
	
	
	//	Test get 
	if ( os_arraylist_get( list, 0 ) != test_item ) 
		return 1;
	
	
	//	Test remove
	if ( os_arraylist_remove( list, test_item ) == 1 ) 
		return 1;
	
	if ( os_arraylist_contains( list, test_item ) == 1 ) 
		return 1;
	
	
	//	Test size limit
	for ( uint32_t i = 1; i < DATA_STRUCTURE_SIZE + 1; i++ ) {
		if ( os_arraylist_add( list, (uint32_t*) i ) != 0 ) 
			return 1;
	}
	
	if ( os_arraylist_add( list, (uint32_t*) test_item ) == 0 )
		return 1;
	
	
	return 0;
} 
