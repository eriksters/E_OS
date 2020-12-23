#include "EOS.h"
#include "EOS_SysCalls.h"
#include "EOS_Scheduler.h"
#include "EOS_Data.h"
#include <stdio.h>

extern os_Control_t os_Control;

void os_init( void ) {
	os_core_init();
	os_scheduler_init();
}

//	TODO: Implement with SCI
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
	stackedRegisters->LR = (uint32_t) 0; // &os_TaskEnd;
	stackedRegisters->PC = (uint32_t) func;
	stackedRegisters->xPSR = 0x01000000;
	
	tcb->SP = ( uint32_t ) stackedRegisters;
	
	
	queue_add( (uint32_t*) tcb );
	//os_Task_Queue[os_Control.taskCount] = tcb;
	os_Control.taskCount++;
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
	os_task_switch();
}

void os_Release_f ( void ) {
	printf("OS Release\n");
	
	os_task_switch();
}

void os_Delay_f( uint32_t milliseconds ) {
	printf("Milliseconds: %d\n", milliseconds);
	
	os_add_to_blocked( (uint32_t*) os_getCurrentTask() );
	
	os_Control.status = block;
	
	os_task_switch();
}

void os_TaskEnd_f ( void ) {

}
