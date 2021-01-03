#ifndef EOS_CORE_ARCH_H
#define EOS_CORE_ARCH_H

#include <stdint.h>
#include "stm32f10x.h"
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

/*
 *
*/
void os_arch_start( void );

/*
 *
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
void os_task_switch_trigger( void );

#endif
