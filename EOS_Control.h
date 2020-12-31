#ifndef EOS_CONTROL_H
#define EOS_CONTROL_H

#include "EOS_Core.h"


/* Extern Instance of os_Control_t that can be accessed by other translation units.
 * Real os_Control instance is in EOS_Control.c
*/
extern os_Control_t os_Control;


/* Get currently executing task (from os_Control).
 * Returns currently running task.
*/ 
os_TCB_t* os_ctrl_get_current_task( void );


/* Get backed up registers data structure from currently running task.
 * Used in assembly code.
*/
os_Registers_t* os_ctrl_get_current_task_reg( void );


/* Returns status of the OS (from os_Control).
 * Used in assembly code.
*/
enum os_status_e os_ctrl_get_status( void );


/* Set OS status as running
 * Used in assembly code.
*/
void os_ctrl_set_status_running( void );

#endif
