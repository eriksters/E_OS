#ifndef EOS_SYSCALLS_H
#define EOS_SYSCALLS_H

#include "EOS_Core.h"

void os_Start_f( void );
void os_Release_f( void );
void os_Delay_f( uint32_t milliseconds );
void os_TaskEnd_f( void );


#endif
