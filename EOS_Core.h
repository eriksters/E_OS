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
extern os_TCB_t os_exit_worker_H;
extern os_TCB_t os_wait_worker_H;



/********************************************/
/*							Functions 									*/
/********************************************/


/* Called by a user selected time source for scheduling and time keeping
 * How often scheduler gets called can be configured by changing OS_TASK_SWITCH_TICK_COUNT in EOS_Config.h
*/
void os_tick( void );


/* Initialize Core data structures.
 * Must be called before tasks are created and OS started.
 * Called automatically from os_init.
 * Param os_tick_frq: Frequency (in HZ) of os_tick calls. 
 * If Sys_Tick is used and configured, system clock is configured and SystemCoreClock is updated, 0 can be passed to calculate os_tick_frq automatically.
*/
void os_core_init( uint32_t os_tick_frq );



#endif
