#include "EOS.h"
#include "EOS_Dispatcher.h"
#include "stm32f10x.h"
#include "EOS_Scheduler.h"

#include <stdio.h>

void os_Start_f( void );
void os_Release_f( void );
void os_TaskEnd_f( void );

void os_Switch_f( void );
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

}

void os_Start ( void ) {
	__asm("SVC #0x00");
}

void os_Start_f( void ) {
	
	printf("OS Start \n");
	
	//	TODO: 
	//	Select which task to run first
	os_Registers_t* nextTask = (os_Registers_t*) queue_remove();
	
	//	Do not start if no tasks have been created
	if ( nextTask == 0 ) {
		return;
	}
	
	os_Control.currentTask = nextTask;
	
	//	Set System to use PSP in thread mode
	__set_CONTROL(0x02);
	
	//	Start the OS
	os_Reg_Restore();
}


void os_Release ( void ) {
	__asm("SVC #0x01");
}

void os_Release_f ( void ) {
	printf("OS Release\n");
	
	os_Reg_Save();
	
	os_Switch();
	
	os_Reg_Restore();
}


void os_TaskEnd ( void ) {
	__asm("SVC #0x02");
}

void os_TaskEnd_f ( void ) {

}
