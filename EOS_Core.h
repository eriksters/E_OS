#ifndef EOS_CORE_H
#define EOS_CORE_H

#include <stdint.h>
#include "EOS_Core_Arch.h"

/********************************************/
/*								Defines										*/
/********************************************/

#define UNUSED(x) (void)(x)

/********************************************/
/*								TypeDefs									*/
/********************************************/

enum status {
	notStarted,
	running,
	block
};


typedef struct {
	uint32_t placeholder[(OS_TASK_STACK_SIZE / 4)];
} os_TaskStack_t;


typedef struct {
	os_Registers_t backed_up_registers;
	uint8_t stack[OS_TASK_STACK_SIZE];
} os_TCB_t;

typedef struct {
	uint32_t taskCount;
	os_TCB_t* currentTask;
	uint32_t tick_counter;
	uint32_t task_switch_tick_count;
	uint32_t os_tick_frq;
	uint32_t status;
} os_Control_t;
 
typedef struct {
	os_TCB_t* testArray[10];
	uint32_t size;
	uint32_t max_size;
} os_tasks_blocked_t;




/********************************************/
/*							Functions 									*/
/********************************************/

void os_tick( void );
void os_init_blocked( void );
os_TCB_t* os_add_to_blocked( os_TCB_t* E );
os_TCB_t* os_remove_from_blocked( os_TCB_t* E );
void os_core_init( uint32_t os_tick_frq );
void os_task_switch( void );

#endif
