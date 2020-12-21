#include "EOS.h"
#include "EOS_Dispatcher.h"
#include "stm32f10x.h"
#include "EOS_Scheduler.h"

#include <stdio.h>

void os_Start_f( void );
void os_Release_f( void );
void os_TaskEnd_f( void );
void os_Delay_f( uint32_t milliseconds );
uint32_t os_GetStatus( void );
void os_SetStarted( void );

typedef struct {
	uint32_t* testArray[10];
	uint32_t size;
	uint32_t max_size;
} blockedDataStructure;
static blockedDataStructure blck;


void initDataStruct( void ) {
	blck.size = 0;
	blck.max_size = 10;
}

uint32_t* addDataStruct( uint32_t* E );
uint32_t* addDataStruct( uint32_t* E ) {
	
	uint32_t* ret = 0;
	
	if ( blck.size >= blck.max_size ) {
		return ret;
	}
	
	if ( E == 0 ) {
		return ret;
	}
	
	for ( uint32_t i = 0; i < blck.max_size; i++ ) {
		if ( blck.testArray[i] == 0 ) {
			blck.testArray[i] = E;
			blck.size++;
			ret = E;
			break;
		}
	}
	
	return ret;
}

uint32_t* removeDataStruct( uint32_t* E );
uint32_t* removeDataStruct( uint32_t* E ) {
	
	uint32_t* ret = 0;
	
	if ( blck.size == 0 ) {
		return ret;
	}
	
	if ( E == 0 ) {
		return ret;
	}
	
	for ( uint32_t i = 0; i < blck.max_size; i++) {
		if ( blck.testArray[i] == E ) {
			blck.testArray[i] = 0;
			blck.size--;
			ret = E;
			break;
		}
	}
	
	return ret;
}

void testDataStruct( void );
void testDataStruct( void ) {
	initDataStruct();
	
	uint32_t* p_1 = (uint32_t*) 0x01;
	uint32_t* p_2 = (uint32_t*) 0x02;
	uint32_t* p_3 = (uint32_t*) 0x03;
	
	addDataStruct( p_1 );
	addDataStruct( p_2 );
	addDataStruct( p_3 );
	
	removeDataStruct( p_1 );
	addDataStruct( p_1 );
	
	removeDataStruct(p_2);
	removeDataStruct(p_3);
	
	addDataStruct( p_3 );
	addDataStruct( p_2 );
	
	
}

void os_TriggerPendSV( void );
void os_TriggerPendSV( void ) {
	SCB->ICSR |= 0x1 << 28;
	__DSB();
	__ISB();
} 

void SVC_Handler_f( os_StackedReg_t* stackedRegisters );
void SVC_Handler_f( os_StackedReg_t* stackedRegisters ) {
	uint16_t* SCI_p = (uint16_t*) stackedRegisters->PC;
	
	uint16_t SCI = *(--SCI_p);
	SCI &= 0xFF;
	
	switch (SCI) {
		
		//	Start
		case 0:
			os_Start_f();
			break;
		
		//	Release 
		case 1:
			os_Release_f();
			break;
		
		//	Delay
		case 3:
			os_Delay_f( stackedRegisters->R0 );
		
	}
}


void os_CreateTask ( void ( *func )( void ), os_TaskStack_t* stack, os_Registers_t* tcb ) {
	
	//	Stack is descending, so Stack Pointer must be set to end of the memory block
	//	The EXC_RETURN automatically restores several registers, which are stored on the task stack,
	//	so the SP is adjusted to make sure this happens correctly
	
	os_StackedReg_t* stackedRegisters = (os_StackedReg_t*) ((( uint8_t* ) stack) + (OS_TASK_STACK_SIZE - sizeof( os_StackedReg_t )));
	
	stackedRegisters->R0 = 0x11223344;			//	TODO: task parameters
	stackedRegisters->R1 = 0x0;
	stackedRegisters->R2 = 0x0;
	stackedRegisters->R3 = 0x0;
	stackedRegisters->R12 = 0x0;
	stackedRegisters->LR = (uint32_t) &os_TaskEnd;
	stackedRegisters->PC = (uint32_t) func;
	stackedRegisters->xPSR = 0x01000000;
	
	tcb->SP = ( uint32_t ) stackedRegisters;
	
	
	queue_add( (uint32_t*) tcb );
	//os_Task_Queue[os_Control.taskCount] = tcb;
	os_Control.taskCount++;
}

void os_DeleteTask ( os_Registers_t * tcb ) {
	tcb = tcb + 1;
}

void os_Start ( void ) {
	__asm("SVC #0x00");
}

void os_Start_f( void ) {
	
	printf("OS Start \n");
	
	//	Do not start if no tasks have been created
	if (os_Control.taskCount < 1) {
		return;
	}
	
	os_Control.task_switch_tick_count = 1;
	os_Control.tick_counter = 0;
	
	//	Set System to use PSP in thread mode
	__set_CONTROL(0x02);
		
	//	Start the OS
	os_TriggerPendSV();
}


void os_Release ( void ) {
	__asm("SVC #0x01");
}

void os_Release_f ( void ) {
	printf("OS Release\n");
	
	/*
	os_Reg_Save();
	
	os_Switch_f();
	
	os_Reg_Restore();
	*/
	
	os_TriggerPendSV();
}

uint32_t os_GetStatus( void ) {
	return (uint32_t) os_Control.status;
}

void os_SetStarted( void ) {
	os_Control.status = running;
}

void os_TaskEnd ( void ) {
	__asm("SVC #0x02");
}

void os_TaskEnd_f ( void ) {

}

void os_tick( void ) {
	os_Control.tick_counter++;
	if ( os_Control.tick_counter >= os_Control.task_switch_tick_count ) {
		printf("Triggering Context Switch\n");
		os_TriggerPendSV();
		os_Control.tick_counter = 0;
	}
}

void os_Delay( uint32_t milliseconds ) {
	UNUSED(milliseconds);
	__asm("SVC #0x3");
}

void os_Delay_f( uint32_t milliseconds ) {
	printf("Milliseconds: %d\n", milliseconds);
	
	addDataStruct( (uint32_t*) os_getCurrentTask() );
	
	os_Control.status = block;
	
	os_TriggerPendSV();
}
