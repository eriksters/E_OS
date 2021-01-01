#include "EOS_DataStructures.h"

uint32_t arrayList_init( arrayList_h handle, void* array, uint32_t max_size ) {
	
	if ( max_size == 0 || array == 0 || handle == 0 ) {
		return 1;
	}
	
	handle->max_size = max_size;
	handle->array = array;
	
	return 0;
}

uint32_t arrayList_add( arrayList_h handle, void* E ) {
	
}

uint32_t arrayList_remove( arrayList_h handle, void* E ) {

}

uint32_t arrayList_contains( arrayList_h handle, void* ptr ) {

}

uint32_t arrayList_size( arrayList_h handle ) {

}
