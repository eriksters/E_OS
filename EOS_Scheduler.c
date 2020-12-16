#include "EOS_Scheduler.h"

int os_queue_isFull( void );

typedef struct {
	uint32_t* q[OS_MAX_TASK_COUNT_];
	uint32_t head;
	uint32_t tail;
	uint32_t size;
	uint32_t max_size;
} queue_t;

static queue_t tq;

void os_Queue_init( void ) {
	tq.size = 0;
	tq.head = 0;
	tq.tail = (OS_MAX_TASK_COUNT_ - 1);
	tq.max_size = OS_MAX_TASK_COUNT_;
}
	

int os_queue_isFull( void ) {
	
	return (tq.max_size == tq.size);
		
}

uint32_t* queue_add( uint32_t* E ) {
	
	if ( os_queue_isFull() ) {
		return 0;
	}
	
	tq.tail++;
	if ( tq.tail == tq.max_size ) {
		tq.tail = 0;
	}
	
	tq.q[tq.tail] = E;
	tq.size++;
	
	return E;
}




void os_Qeueue_TEST( void ) {
	volatile uint32_t* q = 0;
	
	
	
	for (int i = 0; i < 3; i++) {
		q = queue_add( (uint32_t*) (++q) );
	}
	
	for ( int i = 0; i < 3; i++) {
		q = queue_remove();
	} 
	
	for (int i = 0; i < 3; i++) {
		q = queue_add( (uint32_t*) (0x12345670 + i) );
	}
	
	for ( int i = 0; i < 7; i++) {
		q = queue_remove();
	}

	for (int i = 0; i < 10; i++) {
		q = queue_add( (uint32_t*) (0x12345670 + i) );
	}

	for ( int i = 0; i < 3; i++) {
		q = queue_remove();
	} 	
	
}



uint32_t* queue_remove( void ) {
	
	if ( tq.size == 0 ) {
		return 0;
	}
	
	uint32_t* ret = tq.q[tq.head];
	
	tq.head++;
	if ( tq.head == tq.max_size ) {
		tq.head = 0;
	}
	
	tq.size--;
	
	return ret;
	
}


uint32_t* queue_peek( void ) {
	return 0;
}

