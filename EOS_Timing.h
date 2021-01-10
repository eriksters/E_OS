#ifndef EOS_TIMING_H
#define EOS_TIMING_H

#include <stdint.h>

/*	Initialize Data structures required by the timing module
 *	Param os_tick_frq: frequency of os_tick calls. If 0, assumed that SysTick timer is used and os_tick_frq is calculated automatically.
*/
void os_timing_init( uint32_t os_tick_frq );


/* Called by a user selected time source for scheduling and time keeping
 * How often scheduler gets called can be configured by changing OS_TASK_SWITCH_TICK_COUNT in EOS_Config.h
*/
void os_tick( void );


/* Handler for os_delay System Call. 
*/
void os_delay_call_handler( uint32_t milliseconds );

#endif
