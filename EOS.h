#ifndef EOS_H
#define EOS_H

#include <stdint.h>
#include "EOS_Core.h"

 /*******************************************
 *							OS Interface        				*
 *******************************************/

void os_init( void );
void os_CreateTask ( void ( *func )( void ), os_TaskStack_t* stack, os_Registers_t* tcb );
void os_Start ( void );
void os_Release ( void );
void os_Delay ( uint32_t miliseconds );

//	TODO
void os_DeleteTask ( os_Registers_t * tcb );

//	Called when a task reached the end. Something like os_DeleteTask
void os_TaskEnd ( void );



#endif

