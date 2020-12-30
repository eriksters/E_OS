#include "EOS_Core_Arch.h"

void os_arch_create_task( void ( *func )( void ), uint32_t* stack_end, os_Registers_t* reg_backup ) {
	
	os_StackedReg_t* stackedRegisters = (os_StackedReg_t*) ((( uint8_t* ) stack_end) + (OS_TASK_STACK_SIZE - sizeof( os_StackedReg_t )));
	
	stackedRegisters->R0 = 0x11223344;			//	TODO: task parameters
	stackedRegisters->R1 = 0x0;
	stackedRegisters->R2 = 0x0;
	stackedRegisters->R3 = 0x0;
	stackedRegisters->R12 = 0x0;
	stackedRegisters->LR = (uint32_t) 0; // &os_task_end;
	stackedRegisters->PC = (uint32_t) func;
	stackedRegisters->xPSR = 0x01000000;
	
	reg_backup->SP = ( uint32_t ) stackedRegisters;
	
}
