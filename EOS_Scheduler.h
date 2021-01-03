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
void os_schedule_task( os_TCB_t* task );


/* Deschedule a task.
 * Called when a task finishes or when a task is deleted.
 * Implemented in EOS_Sheduler.c
*/
void os_deschedule_task( os_TCB_t* task );


/* Block a task.
 * Implemented in EOS_Sheduler.c
*/
void os_block_task( os_TCB_t* task );


/* Unblock a task.
 * Implemented in EOS_Sheduler.c
*/
void os_unblock_task( os_TCB_t* task );


/* Get amount of blocked tasks.
 * Implemented in EOS_Sheduler.c
*/
uint32_t os_get_blocked_task_amount( void );


/* Get a blocked task by its index.
 * Implemented in EOS_Sheduler.c
*/
os_TCB_t* os_get_blocked_task_by_index( uint32_t index );


// void os_remove_task_from_blocked( os_TCB_t* task );


/* Check if ready task data structure is full. 
 * Returns 1 if full, 0 if not.
 * Implemented in EOS_Scheduler.c
*/ 
// uint32_t os_ready_isFull( void );


/* Add task to ready ready task data structure. 
 * Signifies to scheduler that the task is ready to be scheduled and executed. 
 * Returns E if successful, NULL if not (Not enough space in data structure).
 * Implemented in: EOS_Scheduler.c
*/
// os_TCB_t* os_ready_add( os_TCB_t* E );


/* Get next task to be scheduled and remove it from ready task data structure.
 * Returns removed task if successful, NULL if no tasks are ready.
 * Implemented in: EOS_Scheduler.c
*/
// os_TCB_t* os_ready_remove( void );


/* Get next task to be scheduled but do not remove it from ready task data structure.
 * Returns next task if there is one, NULL if no tasks are ready.
 * Implemented in EOS_Scheduler.c
*/ 
// os_TCB_t* os_ready_peek( void );

#endif
