#ifndef EOS_H
#define EOS_H

#include <stdint.h>
#include "EOS_Core.h"

 /*******************************************
 *							OS Interface        				*
 *******************************************/

/* Handler for SVC interrupt, used for System Calls
 * Should not be called from application.
 * Called from EOS_Dispatcher.s
 * Implemented in EOS.c
*/
void SVC_Handler_f( os_StackedReg_t* stackedRegisters );


/*	Initialize the OS.
 *	Must be called before creating tasks / starting os.
 *  [Param] os_tick_frq: frequency of os_tick calls. If 0, assumed that SysTick timer is used and os_tick_frq is calculated automatically.
 *	Implemented in: EOS_SysCalls.c
*/	
void os_init( uint32_t os_tick_frq );


/*	Create a new task with given program and Task Control Block.
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_task_create ( void ( *func )( void ), os_TCB_t* tcb );


/*	Start the OS.
 *	OS must be initialized and at least one task created for the OS to start.
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_start ( void );


/*	End task's current time slice. Does not block the task.
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_release ( void );


/*	Block current task for X amount of milliseconds
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_delay ( uint32_t miliseconds );


/*	Delete task
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_task_delete ( os_TCB_t * tcb );


/*	Called when a task finishes
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_task_end ( void );


/*	Create/Initialize a mutex.
 *	Returns 0 if successful. (Currently always)
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/
uint32_t os_mutex_create( os_mutex_t* );


/*	Lock a mutex to currently executing task.
 *	Returns 0 if successful. 1 if locked by another task.
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.cs
*/
uint32_t os_mutex_lock( os_mutex_t* );


/*	Unlock mutex.
 *	Returns 0 if successful. 1 if not locked at all or locked by another task.
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.cs
*/
uint32_t os_mutex_unlock( os_mutex_t* );


#endif

