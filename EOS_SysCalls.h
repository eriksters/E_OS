#ifndef EOS_SYSCALLS_H
#define EOS_SYSCALLS_H

#include "EOS_Core.h"

void os_start_f( void );
void os_release_f( void );
void os_delay_f( uint32_t milliseconds );
void os_task_end_f( void );
uint32_t os_mutex_create_f( os_mutex_t* mutex_p );
uint32_t os_mutex_lock_f( os_mutex_t* mutex_p );
uint32_t os_mutex_unlock_f( os_mutex_t* mutex_p );

#endif
