#include "EOS.h"
#include "EOS_Dispatcher.h"
#include "stm32f10x.h"

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
	
	os_Task_Queue[os_Control.taskCount] = tcb;
	os_Control.taskCount++;
}

void os_DeleteTask ( os_Registers_t * tcb ) {

}

void os_Start ( void ) {
	
	//	Set System to use PSP in thread mode
	__set_CONTROL(0x03);
	
	//	Select which task to run next
	//	TODO Replace with scheduler
	
	if (os_Control.currentTask == 0) 
		os_Control.currentTask = 1;
	else 
		os_Control.currentTask = 0;
	
	os_Reg_Restore();
}

void os_Release ( void ) {

}

void os_TaskEnd ( void ) {

}
