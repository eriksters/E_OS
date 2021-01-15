#include "EOS_Core_Arch.h"
#include "stm32f10x.h"

extern void os_task_finished( void );

void os_arch_start( void ) {
	NVIC_SetPriority( PendSV_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL );
	
	if ( OS_PRIVILEGED_TASKS ) { 
		__set_CONTROL(0x02);
		__ISB();
	} else {
		__set_CONTROL(0x03);
		__ISB();
	}
}

void os_arch_exit( void ) {
	__set_CONTROL(0x00);
	__ISB();
}

void os_arch_create_task( void ( *func )( void * ), uint32_t* stack_end, os_Registers_t* reg_backup, void * params ) {

	//	When the task is first dispatched, the registers that would be stacked during exception entry, are loaded from the stack, 
	//	so they need to be set during task creation.
	//	First, find what the stack pointer should be.
	
	//	How many 4-byte integers do the stacked registers take up?
	//	Stack starts at last highest address of the stack. stack_end points to lowest address.
	uint32_t stackedRegOffset = (OS_TASK_STACK_SIZE - sizeof( os_StackedReg_t )) / 4;
	
	//	Add offset to stack_end to get stack pointer
	os_StackedReg_t* sp_stackedReg = (os_StackedReg_t*) (stack_end + stackedRegOffset );
	
	//	Set values of each register when task starts
	sp_stackedReg->R0 = (uint32_t) params;
	sp_stackedReg->R1 = 0x0;
	sp_stackedReg->R2 = 0x0;
	sp_stackedReg->R3 = 0x0;
	sp_stackedReg->R12 = 0x0;
	sp_stackedReg->LR = (uint32_t) &os_task_finished; 
	sp_stackedReg->PC = (uint32_t) func;
	sp_stackedReg->xPSR = 0x01000000;
	
	reg_backup->SP = ( uint32_t ) sp_stackedReg;
	
}


void os_trigger_task_switch( void ) {
	SCB->ICSR |= 0x1 << 28;
	__DSB();
	__ISB();
} 

__attribute__((weak)) void os_tick_reset( void ) {
	SysTick->VAL = 0;
	
	NVIC_ClearPendingIRQ(SysTick_IRQn);
}

uint32_t os_get_systick_frq( void ) {
	uint32_t coreClk = SystemCoreClock;
	uint32_t sysTickResetVal = SysTick->LOAD;
		
	return coreClk / sysTickResetVal;
}

