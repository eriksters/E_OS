#ifndef EOS_TASKS_H
#define EOS_TASKS_H

#include <stdint.h>
#include "EOS_Core_Arch.h"

/*	State of a task.
 *	Allowed values: OS_TASK_STATE_* from EOS_Defines.h
*/
typedef uint32_t os_task_state_t;

/* Task Control Block.
 * os_TCB_t* is used as task handle.
*/
typedef struct {
	os_task_state_t state;
	os_Registers_t backed_up_registers;
	uint32_t stack[OS_TASK_STACK_SIZE / 4];
	uint32_t countdown;
} os_TCB_t;

typedef os_TCB_t* os_task_h;


/* Initialize Tasks Module
*/
void os_tasks_init( void );


/* Handler for os_create_task System Call.
*/
os_task_h os_create_task_call_handler ( void ( *func )( void * ), os_TCB_t* tcb, void * params );


/* Handler for os_delete_task System Call.
*/
void os_delete_task_call_handler ( os_task_h tcb );


/*	Called when the scheduler has removed the task
*/
void os_task_deleted( void );
	

/* Handler for os_task_finished System Call.  
*/
void os_task_finished_call_handler( void );


/* Get currently executing task (from os_Control).
 * Returns currently running task.
*/ 
os_task_h os_get_current_task( void );


/* Set Current Task's state as running
 * Used in assembly code
*/
void os_set_current_task_state( os_task_state_t state );


/* Get backed up registers data structure from currently running task.
 * Used in assembly code.
*/
os_Registers_t* os_get_current_task_reg( void );


/* Set current task in os_Control.
*/
void os_set_current_task( os_task_h task );


/* Get amount of created and not deleted tasks
*/
uint32_t os_get_task_count( void );

#endif
