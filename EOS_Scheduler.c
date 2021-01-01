#include "EOS_Scheduler.h"
#include "EOS_Control.h"
#include <stdio.h>


typedef struct {
	os_TCB_t* q[OS_MAX_TASK_COUNT];
	uint32_t head;
	uint32_t tail;
	uint32_t size;
	uint32_t max_size;
} os_tasks_ready_t;

static os_tasks_ready_t os_tasks_ready;

void os_scheduler_init( void ) {
	os_tasks_ready.size = 0;
	os_tasks_ready.head = 0;
	os_tasks_ready.tail = (OS_MAX_TASK_COUNT - 1);
	os_tasks_ready.max_size = OS_MAX_TASK_COUNT;
}


	

int os_ready_isFull( void ) {
	
	return (os_tasks_ready.max_size == os_tasks_ready.size);
		
}

os_TCB_t* os_ready_add( os_TCB_t* E ) {
	
	if ( os_ready_isFull() ) {
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


os_TCB_t* os_ready_remove( void ) {
	
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


os_TCB_t* os_ready_peek( void ) {
	
	if ( os_tasks_ready.size == 0 )
			return 0;
	
	return os_tasks_ready.q[os_tasks_ready.head];
}


/*	Result is impacted by os state:
 *	STARTING: Only picks the next task
 *  RUNNING: Picks the next task and adds previous task back in queue
 *
 *	If previous task has been blocked or is in zombie state, do not add it back in queue.
 *	If next task is blocked or in zombie state, do not set it as current task.
 *  If next task is in zombie state, remove it from scheduling and set as deleted.
*/
void os_switch_f( void ) {
	
	os_TCB_t* previousTask = 0;
	os_TCB_t* nextTask = 0;
	
	
	if ( os_Control.state != OS_STATE_STARTING ) {
		
		previousTask = os_ctrl_get_current_task();
		
		//	Add previous task to the end of the queue, if it has not been deleted or blocked
		if ( previousTask->state == OS_TASK_STATE_RUNNING ) {
			os_ready_add( previousTask );
			previousTask->state = OS_TASK_STATE_READY;
		
		//	If previous Task is in zombie state, change it to deleted
		} else if ( previousTask->state == OS_TASK_STATE_ZOMBIE ) {
			previousTask->state = OS_TASK_STATE_DELETED;
		}
	}
	
	
	/*
	//	Do nothing if there are no other tasks to run
	//	TODO: queue can be empty while all tasks are blocked
	if (nextTask == 0) {
		return;
	}
	*/
	
	
	
	
	do {
		nextTask = os_ready_remove();
		
		if ( nextTask == 0 ) {
			
			if ( os_blocked_size() == 0 ) {
				os_Control.state = OS_STATE_EXIT;
				
				//	TODO: Exit task
				
				return;
				
			} else {
				//	TODO: Dummy task to execute while other tasks are blocked
			}
			
			
		}
		
	} while ( nextTask->state == OS_TASK_STATE_ZOMBIE || nextTask->state == OS_TASK_STATE_BLOCKED );
	
	
	
	/*
	while ( nextTask->state == OS_TASK_STATE_ZOMBIE || nextTask->state == OS_TASK_STATE_BLOCKED ) {
		
		if ( nextTask->state == OS_TASK_STATE_ZOMBIE ) {
			nextTask->state = OS_TASK_STATE_DELETED;
		}
		
		nextTask = os_ready_remove();
		
		if ( nextTask == 0 ) {
			return;
		} 
	}
	*/
	
	//	Set next task as current task
	os_Control.currentTask = nextTask;
}


