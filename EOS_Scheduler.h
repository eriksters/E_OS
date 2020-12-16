#ifndef EOS_SCHEDULER_H
#define EOS_SCHEDULER_H

#include <stdint.h>

#define OS_MAX_TASK_COUNT_ 10

void os_Queue_init( void );

void os_Switch( void );

//	Add element to queue. Returns inserted element if successful, NULL if queue already full
uint32_t* queue_add( uint32_t* E );

//	Retrieve head Element and remove from queue
uint32_t* queue_remove( void );

//	Retrieve head element and do not remove from queue
uint32_t* queue_peek( void );

#endif
