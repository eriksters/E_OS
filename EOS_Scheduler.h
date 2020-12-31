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
void os_switch_f( void );


/* Check if ready task data structure is full. 
 * Returns 1 if full, 0 if not.
 * Implemented in EOS_Scheduler.c
*/ 
int os_ready_isFull( void );


/* Add task to ready ready task data structure. 
 * Signifies to scheduler that the task is ready to be scheduled and executed. 
 * Returns E if successful, NULL if not (Not enough space in data structure).
 * Implemented in: EOS_Scheduler.c
*/
os_TCB_t* os_ready_add( os_TCB_t* E );


/* Get next task to be scheduled and remove it from ready task data structure.
 * Returns removed task if successful, NULL if no tasks are ready.
 * Implemented in: EOS_Scheduler.c
*/
os_TCB_t* os_ready_remove( void );


/* Get next task to be scheduled but do not remove it from ready task data structure.
 * Returns next task if there is one, NULL if no tasks are ready.
 * Implemented in EOS_Scheduler.c
*/ 
os_TCB_t* os_ready_peek( void );

#endif
