#ifndef EOS_SETUP_H
#define EOS_SETUP_H

#include "EOS_Core.h"

/* Handler for os_init System Call.
*/
void os_init_call_handler( uint32_t os_tick_frq );


/* Handler for os_start System Call.
*/ 
void os_start_call_handler( void );


/* Handler for os_exit System Call.
*/
void os_exit_call_handler( void );




#endif
