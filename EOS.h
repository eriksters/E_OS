#ifndef EOS_H
#define EOS_H

#include <stdint.h>

/*******************************************
 *								Defines 				 				 *
 ******************************************/

#define OS_TASK_STACK_SIZE		0x200
#define OS_MAX_TASK_COUNT 		10


/*******************************************
 *								TypeDefs				 				 *
 ******************************************/
 
 /*--------- Architecture Specific -------*/
 
typedef struct {
	uint32_t R4;
	uint32_t R5;
	uint32_t R6;
	uint32_t R7;
	uint32_t R8;
	uint32_t R9;
	uint32_t R10;
	uint32_t R11;
	uint32_t SP;
} os_Registers_t;

typedef struct {
	uint32_t R0;
	uint32_t R1;
	uint32_t R2;
	uint32_t R3;
	uint32_t R12;
	uint32_t LR;
	uint32_t PC;
	uint32_t xPSR;
} os_StackedReg_t;


 /*--------------- General ---------------*/
 
typedef struct {
	uint32_t taskCount;
	os_Registers_t* currentTask;
	uint8_t isStarted;
} os_Control_t;
 
typedef struct {
	uint32_t placeholder[(OS_TASK_STACK_SIZE / 4)];
} os_TaskStack_t;

 

 /*******************************************
 *							  OS Data				     				*
 *******************************************/

extern os_Control_t os_Control;
extern os_Registers_t* os_Task_Queue[OS_MAX_TASK_COUNT];

 /*******************************************
 *							OS Interface        				*
 *******************************************/

void os_CreateTask ( void ( *func )( void ), os_TaskStack_t* stack, os_Registers_t* tcb );
void os_DeleteTask ( os_Registers_t * tcb );
void os_Start ( void );
void os_Release ( void );
void os_Delay ( uint32_t miliseconds );


//	Called when a task reached the end. Something like os_DeleteTask
void os_TaskEnd ( void );

#endif

