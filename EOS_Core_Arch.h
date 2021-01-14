#ifndef EOS_CORE_ARCH_H
#define EOS_CORE_ARCH_H

#include <stdint.h>
#include "EOS_Config.h"

/*******************************************/
/* 								TypeDefs								 */
/*******************************************/

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


/*******************************************/
/* 								Functions 							 */
/*******************************************/

/* Initialize hardware.
 * Called during OS startup.
*/
void os_arch_start( void );


/* Deinitialize hardware.
 * Called during OS shut down.
*/
void os_arch_exit( void );
	

/* Initialize a task's backed up registers to hardware requirements.
*/
void os_arch_create_task( void ( *func )( void * ), uint32_t* stack, os_Registers_t* regs, void * params );


/* Resets the os_tick time source to initial countdown value.
 * By default, resets Sys_Tick->VAL to 0, then clears SysTick pending interrupt. 
 * Has as a 'weak' attribute, so it can be overwritten by user if another time source is used.
*/
void os_tick_reset( void );


/* Triggers task switch on interrupt exit. 
 * In Cortex-M3: sets PendSV as pending.
*/
void os_trigger_task_switch( void );


/* Get frequency of SysTick interrupts in HZ
 * In Cortex-M3: sets PendSV as pending.
*/
uint32_t os_get_systick_frq( void );

#endif
