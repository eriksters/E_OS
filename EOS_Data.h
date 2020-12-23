#ifndef EOS_DATA_H
#define EOS_DATA_H

#include "EOS_Core.h"

os_TCB_t* os_getCurrentTask( void );
uint32_t os_GetStatus( void );
void os_SetStarted( void );

#endif
