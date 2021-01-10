#ifndef EOS_MUTEX_H
#define EOS_MUTEX_H

#include "EOS_Core.h"

typedef struct {
	os_task_h owner;
} os_mutex_t;

/* Mutex Handle type
 * Returned by Successful os_create_mutex System Call
*/
typedef os_mutex_t* os_mutex_h;

/* Initialize data structures required by mutexes
*/
void os_mutex_init( void );


/* Handler for os_create_mutex System Call.  
*/
os_mutex_h os_create_mutex_call_handler( os_mutex_t* mutex_p );


/* Handler for os_lock_mutex System Call.  
*/
uint32_t os_lock_mutex_call_handler( os_mutex_h mutex );


/* Handler for os_unlock_mutex System Call.  
*/
uint32_t os_unlock_mutex_call_handler( os_mutex_h mutex );


/* Unlock all mutexes that are locked by specified task
*/
void os_unlock_all_mutexes_by_task( os_task_h task );

#endif
