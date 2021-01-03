#ifndef EOS_CORE_H
#define EOS_CORE_H

#include <stdint.h>
#include "EOS_Core_Arch.h"
#include "EOS_Defines.h"
#include "EOS_DataStructures.h"

/********************************************/
/*								Defines										*/
/********************************************/

#define UNUSED(x) (void)(x)

/********************************************/
/*								TypeDefs									*/
/********************************************/


/* Task Control Block.
 * os_TCB_t* is used as task handle.
*/
typedef struct {
	uint32_t state;
	uint32_t countdown;
	os_Registers_t backed_up_registers;
	uint32_t stack[OS_TASK_STACK_SIZE / 4];
} os_TCB_t;


/* Contains state and configuration of the OS.
*/
typedef struct {
	uint32_t taskCount;
	os_TCB_t* currentTask;
	uint32_t tick_counter;
	uint32_t task_switch_tick_count;
	uint32_t os_tick_frq;
	uint32_t state;
} os_Control_t;


typedef uint32_t os_State_t;

typedef uint32_t os_Task_State_t;



/* Task Control Block.
 * os_TCB_t* is used as task handle.
*/
typedef struct {
	os_TCB_t* owner;
} os_mutex_t;



/********************************************/
/*								Data 											*/
/********************************************/

extern os_arrayList_h os_mutex_arraylist_handle;
extern os_arrayList_h os_blocked_arrayList_handle;
extern os_TCB_t os_exit_task_handle;
extern os_TCB_t os_wait_task_handle;



/********************************************/
/*							Functions 									*/
/********************************************/


/* Called by a user selected time source for scheduling and time keeping
 * How often scheduler gets called can be configured by changing OS_TASK_SWITCH_TICK_COUNT in EOS_Config.h
*/
void os_tick( void );


/* Add task to blocked task data structure.
 * Returns E if successful, NULL if data structure is full or NULL is passed as E.
*/
os_TCB_t* os_blocked_add( os_TCB_t* E );


/* Remove task from blocked task data structure.
 * Returns E if successful, NULL if task was not found or NULL passed as E.
*/
os_TCB_t* os_blocked_remove( os_TCB_t* E );


/* Get amount of blocked tasks.
*/
uint32_t os_blocked_size( void );


/* Get blocked task by index.
*/
os_TCB_t* os_blocked_get( uint32_t index );


/* Resume a blocked task.
 * Removes task from blocked data structure and sets it up for execution.
*/
void os_task_blocked_resume( os_TCB_t* task);


/* Initialize Core data structures.
 * Must be called before tasks are created and OS started.
 * Called automatically from os_init.
 * Param os_tick_frq: Frequency (in HZ) of os_tick calls. 
 * If Sys_Tick is used and configured, system clock is configured and SystemCoreClock is updated, 0 can be passed to calculate os_tick_frq automatically.
*/
void os_core_init( uint32_t os_tick_frq );






#endif
