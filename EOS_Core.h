#ifndef EOS_CORE_H
#define EOS_CORE_H

#include "EOS_Core.h"
#include "EOS_Core_Arch.h"
#include "EOS_Defines.h"
#include "EOS_DataStructures.h"
#include "EOS_Config.h"
#include "EOS_Timing.h"
#include "EOS_Tasks.h"
#include "EOS_Scheduling.h"
#include <stdint.h>

/********************************************/
/*								Defines										*/
/********************************************/

#define UNUSED(x) (void)(x)


/********************************************/
/*								TypeDefs									*/
/********************************************/

/* Type for current OS state
 * Allowed values: OS_STATE_* from EOS_DEFINES.h
*/
typedef uint32_t os_state_t;


/********************************************/
/*									Data		 								*/
/********************************************/

/* Extern Instance of os_Control_t that can be accessed by other translation units.
 * Real os_Control instance is in EOS_Core.c
*/
// extern os_Control_t os_Control;


/********************************************/
/*								Functions 								*/
/********************************************/


/* Initialize Core data structures.
 * Must be called before tasks are created and OS started.
 * Called automatically from os_init.
 * Param os_tick_frq: Frequency (in HZ) of os_tick calls. 
 * If Sys_Tick is used and configured, system clock is configured and SystemCoreClock is updated, 0 can be passed to calculate os_tick_frq automatically.
*/
//	void os_core_init( void );


/* Returns status of the OS (from os_Control).
 * Used in assembly code.
*/
os_state_t os_get_state( void );


/* Set OS state.
 * Used in assembly code.
*/
void os_set_state( os_state_t state );


/* Handler for os_init System Call.
*/
void os_init_call_handler( uint32_t os_tick_frq );


/* Handler for os_start System Call.
*/ 
void os_start_call_handler( void );


/* Handler for os_exit System Call.
*/
void os_exit_call_handler( void );

#endif
