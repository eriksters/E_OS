#ifndef EOS_SCHEDULER_H
#define EOS_SCHEDULER_H

#include <stdint.h>
#include "EOS_Core.h"

/* Initialize scheduler data structures.
 * Must be called before tasks are added to ready queue.
 * Implemented in EOS_Scheduler.c
*/ 
void os_scheduler_init( void );


/* Call scheduler to select and set next task in os_Control.
 * Implemented in EOS_Scheduler.c
*/ 
void os_switch_current_task( void );


/* Schedule a task.
 * Called when a task is first created and when a task is unblocked.
 * Implemented in EOS_Sheduler.c
*/
void os_schedule_task( os_task_h task );


/* Deschedule a task.
 * Called when a task finishes or when a task is deleted.
 * Implemented in EOS_Sheduler.c
*/
void os_deschedule_task( os_task_h task );


/* Block a task.
 * Implemented in EOS_Sheduler.c
*/
void os_block_task( os_task_h task );


/* Unblock a task.
 * Implemented in EOS_Sheduler.c
*/
void os_unblock_task( os_task_h task );


/* Get amount of blocked tasks.
 * Implemented in EOS_Sheduler.c
*/
uint32_t os_get_blocked_task_amount( void );


/* Get a blocked task by its index.
 * Implemented in EOS_Sheduler.c
*/
os_task_h os_get_blocked_task_by_index( uint32_t index );


#endif
