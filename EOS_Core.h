#ifndef EOS_CORE_H
#define EOS_CORE_H

#include <stdint.h>
#include "EOS_Core_Arch.h"

/********************************************/
/*								Defines										*/
/********************************************/

#define UNUSED(x) (void)(x)

#define OS_TASK_STACK_SIZE		0x200
#define OS_MAX_TASK_COUNT 		10

/********************************************/
/*								TypeDefs									*/
/********************************************/

enum status {
	notStarted,
	running,
	block
};

typedef struct {
	uint32_t taskCount;
	os_Registers_t* currentTask;
	uint32_t tick_counter;
	uint32_t task_switch_tick_count;
	uint8_t status;
} os_Control_t;
 
typedef struct {
	uint32_t placeholder[(OS_TASK_STACK_SIZE / 4)];
} os_TaskStack_t;

typedef struct {
	uint32_t* testArray[10];
	uint32_t size;
	uint32_t max_size;
} os_tasks_blocked_t;

typedef struct {
	os_Registers_t backed_up_registers;
} os_TCB_t;


/********************************************/
/*							Functions 									*/
/********************************************/

void os_tick( void );
void os_init_blocked( void );
uint32_t* os_add_to_blocked( uint32_t* E );
uint32_t* os_remove_from_blocked( uint32_t* E );


#endif
