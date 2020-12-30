#ifndef EOS_SCHEDULER_H
#define EOS_SCHEDULER_H

#include <stdint.h>
#include "EOS_Core.h"


void os_scheduler_init( void );

void os_switch_f( void );

int os_queue_isFull( void );

//	Add element to queue. Returns inserted element if successful, NULL if queue already full
os_TCB_t* queue_add( os_TCB_t* E );

//	Retrieve head Element and remove from queue
os_TCB_t* queue_remove( void );

//	Retrieve head element and do not remove from queue
os_TCB_t* queue_peek( void );

#endif
