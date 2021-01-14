#ifndef EOS_SCHEDULING_H
#define EOS_SCHEDULING_H

#include "EOS_Tasks.h"


/******************************************
*				EOS_Scheduling_{algorithm}.c			*
******************************************/

/* Initialize scheduler data structures associated with specific scheduling algorithm implementation.
*/
void os_scheduling_init( void );


/* Called every os_tick from the os_tick function
*/
void os_scheduling_tick( void );


/* Schedule a task (task is ready for execution)
*/
void os_schedule_task( os_task_h task );


/* Deschedule a task (task no longer executable)
*/
void os_deschedule_task( os_task_h task );


/* Call scheduler to select and set next task in os_Control.
*/ 
void os_switch_current_task( void );


/* Handler for os_release System Call.
*/
void os_release_call_handler( void );

#endif
