#include "stm32f10x.h"
#include <stdio.h>

/* Slightly modified version of Clock Setup from example project */
void SystemCoreClockConfigure( void );
void SystemCoreClockConfigure( void ) {
	
  RCC->CR |= ((uint32_t)RCC_CR_HSION);                     // Enable HSI
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                  // Wait for HSI Ready

  RCC->CFGR = RCC_CFGR_SW_HSI;                             // HSI is system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);  // Wait for HSI used as system clock

  FLASH->ACR  = FLASH_ACR_PRFTBE;                          // Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_LATENCY;                         // Flash 1 wait state

  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                         // HCLK = SYSCLK
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;                        // APB1 = HCLK/2
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;                        // APB2 = HCLK

  RCC->CR &= ~RCC_CR_PLLON;                                // Disable PLL

  //  PLL configuration:  = HSI/2 * 16 = 64 MHz
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
  RCC->CFGR |=  (RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL16);

  RCC->CR |= RCC_CR_PLLON;                                 // Enable PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();           // Wait till PLL is ready

  RCC->CFGR &= ~RCC_CFGR_SW;                               // Select PLL as system clock source
  RCC->CFGR |=  RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // Wait till PLL is system clock src
}
//

void EXTI0_IRQHandler( void );
void EXTI0_IRQHandler( void ) {
	printf("EXTI 0 \n");
}

void SysTick_Handler( void );
void SysTick_Handler( void ) {
	printf("In SysTick_Handler\n");
}

typedef struct {
	uint32_t R0;
	uint32_t R1;
	uint32_t R2;
	uint32_t R3;
	uint32_t R4;
	uint32_t R5;
	uint32_t R6;
	uint32_t R7;
	uint32_t R8;
	uint32_t R9;
	uint32_t R10;
	uint32_t R11;
	uint32_t R12;
	uint32_t SP;
	uint32_t LR;
	uint32_t PC;
	uint32_t xPSR;
} os_Registers;

#define TASK_STACK_SIZE		0x200
#define MAX_TASK_COUNT 		10

os_Registers* task_queue[MAX_TASK_COUNT];
static uint32_t taskCount;
static uint32_t currentTask = 0;

extern void Sys_Call( os_Registers* );
extern void SwitchPSP( void );
void init_task( void ( *func )( void ), uint8_t* stack, os_Registers* regs );
extern void start_task( os_Registers* regs );
extern void os_start( void );
void os_release( void );
extern void os_switch( os_Registers* current_task, os_Registers* target_task);
//  void os_addToQueue( os_Registers* regs);


//	Initialize task's banked registers
void init_task( void ( *func )( void ), uint8_t* stack, os_Registers* regs) {
	
	regs->PC = ( uint32_t ) func;
	
	regs->LR = ( uint32_t ) func;				//	TODO - Task End
	
	regs->xPSR = 0x01000000;						//	Always in Thumb state
	
	stack += TASK_STACK_SIZE;
	regs->SP = ( uint32_t ) stack;
	
	task_queue[taskCount] = regs;
	taskCount++;
}

void os_release( void ){
	
	printf("OS release\n");
	
	os_Registers* target_regs;
	os_Registers* current_regs;
	if (currentTask == 0) {
		target_regs = task_queue[1];
		current_regs = task_queue[0];
		currentTask = 1;
	} else {
		target_regs = task_queue[0];
		current_regs = task_queue[1];
		currentTask = 0;
	}
	
	os_switch(current_regs, target_regs);
}



os_Registers t1_reg;
uint8_t t1_stack[TASK_STACK_SIZE];
void t1_func( void ) __attribute__((noreturn)); 

os_Registers t2_reg;
uint8_t t2_stack[TASK_STACK_SIZE];
void t2_func( void ) __attribute__((noreturn));


void t1_func( void ) {
	for (;;) {
		for (int j = 0; j < 10; j++) {		//	Execute 10 times and then release
			printf("t1 func. Count = %d\n", j);
			for (int i = 0; i < 10000; i++) {
				__NOP();
			}
		}
		os_release();
	}
}

void t2_func( void ) {
	for (;;) {
		for (int j = 0; j < 20; j++) {		//	Execute 20 times and then release
			printf("t2 func. Count = %d\n", j);
			for (int i = 0; i < 10000; i++) {
				__NOP();
			}
		}
		os_release();
	}
}

int main() {
	//	Clock setup
	SystemCoreClockConfigure();
	// SysTick_Config(0x00FFFFFF);
	
	//	Switch to PSP
	//  SwitchPSP();
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPendingIRQ(EXTI0_IRQn);
	
	init_task(&t1_func, t1_stack, &t1_reg);
	init_task(&t2_func, t2_stack, &t2_reg);
	
	os_start();
	
	//  start_task(&t1_reg);
	
	
	//	Trigger System Call
	Sys_Call(&t1_reg);
	
	for (;;) {
		printf("Hello world\n");
	}
}
