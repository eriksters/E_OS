#ifndef EOS_H
#define EOS_H

#include <stdint.h>
#include "EOS_Core.h"

 /*******************************************
 *							OS Interface        				*
 *******************************************/

/*	Initialize the OS.
 *	Must be called before creating tasks / starting os.
 *	Implemented in: EOS_SysCalls.c
*/	
void os_init( void );


/*	Create a new task with given program, stack pointer and Task Control Block.
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_CreateTask ( void ( *func )( void ), os_TaskStack_t* stack, os_Registers_t* tcb );


/*	Start the OS.
 *	OS must be initialized and at least one task created for the OS to start.
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_Start ( void );


/*	End task's current time slice. Does not block the task.
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_Release ( void );


/*	Block current task for X amount of milliseconds
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_Delay ( uint32_t miliseconds );


/*	Delete task
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_DeleteTask ( os_Registers_t * tcb );


/*	Called when a task finishes
 *	Implemented in: EOS.c
 *	Handled in: EOS_SysCalls.c
*/	
void os_TaskEnd ( void );



#endif

