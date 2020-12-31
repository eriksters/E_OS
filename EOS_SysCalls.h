#ifndef EOS_SYSCALLS_H
#define EOS_SYSCALLS_H

#include "EOS_Core.h"

/* Handler for os_start System Call.
*/ 
void os_start_f( void );


/* Handler for os_task_create
*/
void os_task_create_f ( void ( *func )( void * ), os_TCB_t* tcb, void * params );


/* Handler for os_release System Call.
*/
void os_release_f( void );


/* Handler for os_delay System Call. 
*/
void os_delay_f( uint32_t milliseconds );


/* Handler for os_task_end System Call.  
*/
void os_task_end_f( void );


/* Handler for os_mutex_create System Call.  
*/
uint32_t os_mutex_create_f( os_mutex_t* mutex_p );


/* Handler for os_mutex_lock System Call.  
*/
uint32_t os_mutex_lock_f( os_mutex_t* mutex_p );


/* Handler for os_mutex_unlock System Call.  
*/
uint32_t os_mutex_unlock_f( os_mutex_t* mutex_p );

#endif
