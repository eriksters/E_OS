#ifndef EOS_SCHEDULER_H
#define EOS_SCHEDULER_H

#include <stdint.h>
#include "EOS_Core.h"

#define OS_MAX_TASK_COUNT_ 10


void os_scheduler_init( void );

void os_Switch_f( void );

int os_queue_isFull( void );

void os_tick( void );

void os_TriggerPendSV( void );

//	Add element to queue. Returns inserted element if successful, NULL if queue already full
uint32_t* queue_add( uint32_t* E );

//	Retrieve head Element and remove from queue
uint32_t* queue_remove( void );

//	Retrieve head element and do not remove from queue
uint32_t* queue_peek( void );

#endif
