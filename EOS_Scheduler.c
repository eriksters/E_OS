#include "EOS_Scheduler.h"
#include <stdio.h>


typedef struct {
	os_TCB_t* q[OS_MAX_TASK_COUNT];
	uint32_t head;
	uint32_t tail;
	uint32_t size;
	uint32_t max_size;
} os_tasks_ready_t;



extern os_Control_t os_Control;


static os_tasks_ready_t os_tasks_ready;

void os_scheduler_init( void ) {
	os_tasks_ready.size = 0;
	os_tasks_ready.head = 0;
	os_tasks_ready.tail = (OS_MAX_TASK_COUNT - 1);
	os_tasks_ready.max_size = OS_MAX_TASK_COUNT;
}

/*
void testDataStruct( void );
void testDataStruct( void ) {
	os_init_blocked();
	
	uint32_t* p_1 = (uint32_t*) 0x01;
	uint32_t* p_2 = (uint32_t*) 0x02;
	uint32_t* p_3 = (uint32_t*) 0x03;
	
	os_add_to_blocked( p_1 );
	os_add_to_blocked( p_2 );
	os_add_to_blocked( p_3 );
	
	os_remove_from_blocked( p_1 );
	os_add_to_blocked( p_1 );
	
	os_remove_from_blocked(p_2);
	os_remove_from_blocked(p_3);
	
	os_add_to_blocked( p_3 );
	os_add_to_blocked( p_2 );
}
*/





	

int os_queue_isFull( void ) {
	
	return (os_tasks_ready.max_size == os_tasks_ready.size);
		
}

os_TCB_t* queue_add( os_TCB_t* E ) {
	
	if ( os_queue_isFull() ) {
		return 0;
	}
	
	os_tasks_ready.tail++;
	if ( os_tasks_ready.tail == os_tasks_ready.max_size ) {
		os_tasks_ready.tail = 0;
	}
	
	os_tasks_ready.q[os_tasks_ready.tail] = E;
	os_tasks_ready.size++;
	
	return E;
}


os_TCB_t* queue_remove( void ) {
	
	if ( os_tasks_ready.size == 0 ) {
		return 0;
	}
	
	os_TCB_t* ret = os_tasks_ready.q[os_tasks_ready.head];
	
	os_tasks_ready.head++;
	if ( os_tasks_ready.head == os_tasks_ready.max_size ) {
		os_tasks_ready.head = 0;
	}
	
	os_tasks_ready.size--;
	
	return ret;
}


os_TCB_t* queue_peek( void ) {
	
	if ( os_tasks_ready.size == 0 )
			return 0;
	
	return os_tasks_ready.q[os_tasks_ready.head];
}



void os_Switch_f( void ) {
	
	os_TCB_t* nextTask = queue_remove();
	
	//	Do nothing if there are no other tasks to run
	//	TODO: queue can be empty while all tasks are blocked
	if (nextTask == 0) {
		return;
	}
	
	//	Add previous task to the end of the queue
	if ( os_Control.status != block ) {
		queue_add( os_Control.currentTask );
	}
	
	//	Set next task as current task
	os_Control.currentTask = nextTask;
}
