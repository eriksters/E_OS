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

typedef uint32_t os_state_t;
typedef uint32_t os_task_state_t;

/* Task Control Block.
 * os_TCB_t* is used as task handle.
*/
typedef struct {
	os_task_state_t state;
	uint32_t countdown;
	os_Registers_t backed_up_registers;
	uint32_t stack[OS_TASK_STACK_SIZE / 4];
} os_TCB_t;

typedef os_TCB_t* os_task_h;

/* Contains state and configuration of the OS.
*/
typedef struct {
	os_state_t state;
	uint32_t taskCount;
	os_task_h currentTask;
	uint32_t tick_counter;
	uint32_t task_switch_tick_count;
	uint32_t os_tick_frq;
} os_Control_t;


/* Task Control Block.
 * os_TCB_t* is used as task handle.
*/
typedef struct {
	os_task_h owner;
} os_mutex_t;

typedef os_mutex_t* os_mutex_h;


/********************************************/
/*								Data 											*/
/********************************************/

extern os_arrayList_h os_mutex_arraylist_handle;
extern os_arrayList_h os_blocked_arrayList_handle;



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
