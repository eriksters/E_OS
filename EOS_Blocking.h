#ifndef EOS_BLOCKING_H
#define EOS_BLOCKING_H

#include "EOS_Core.h"

/* Initialize blocking data structures.
 * Must be called before tasks are added to ready queue.
*/ 
void os_blocking_init( void );



/* Block a task.
*/
void os_block_task( os_task_h task );


/* Unblock a task and schedule it for execution
*/
void os_unblock_task( os_task_h task );


/* Remove task from blocked tasks arraylist, but do not schedule for execution
*/
uint32_t os_remove_task_from_blocked( os_task_h task );


/* Handler for the os_block System Call.
*/
void os_block_call_handler( void );


/*	Handler for the os_unblock System Call.
*/	
void os_unblock_call_handler( os_task_h task );


/*	Get amound of currently blocked tasks.
*/
uint32_t os_get_blocked_task_amount( void );


#endif
