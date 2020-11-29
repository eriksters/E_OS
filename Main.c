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

#define TASK_STACK_SIZE		0x200
#define MAX_TASK_COUNT 		10

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
} os_Registers;

typedef struct {
	uint32_t R0;
	uint32_t R1;
	uint32_t R2;
	uint32_t R3;
	uint32_t R12;
	uint32_t LR;
	uint32_t PC;
	uint32_t xPSR;
} os_stackedReg;

typedef struct {
	uint32_t taskCount;
	uint32_t currentTask;
} os_control;

static os_control control;
os_Registers* task_queue[MAX_TASK_COUNT];

extern void Sys_Call( os_Registers* );
extern void SwitchPSP( void );
void init_task( void ( *func )( void ), uint8_t* stack, os_Registers* regs );
extern void start_task( os_Registers* regs );
void os_start( void );
void os_release( void );
extern void os_switch( os_Registers* current_task, os_Registers* target_task);
void os_task_end( void );

void os_task_end( void ) {								//	TODO
	printf("Task end\n");
}

void SVC_Handler( void );
void SVC_Handler( void ) {
	__set_CONTROL(0x3);											//	Thread mode uses PSP and has Unprivileged Access Level
	__ASM(
		"MOV 	r0, lr\n"
		"AND	r0, #0xFFFFFFF0 \n"
		"ORR	r0, #0x0000000D \n"
		"MOV	lr, r0\n"
		"MOV	r0, sp\n"
		"MSR 	PSP, r0"
	);
	
}

//	Initialize task
void init_task( void ( *func )( void ), uint8_t* stack, os_Registers* regs) {
	
	stack += (TASK_STACK_SIZE - sizeof( os_stackedReg ));
	
	os_stackedReg* stackedRegisters = ( os_stackedReg* ) stack;
	
	stackedRegisters->R0 = 0x11223344;			//	TODO: task parameters
	stackedRegisters->R1 = 0x0;
	stackedRegisters->R2 = 0x0;
	stackedRegisters->R3 = 0x0;
	stackedRegisters->R12 = 0x0;
	stackedRegisters->LR = (uint32_t) &os_task_end;
	stackedRegisters->PC = (uint32_t) func;
	stackedRegisters->xPSR = 0;
	
	regs->SP = ( uint32_t ) stack;
	
	task_queue[control.taskCount] = regs;
	control.taskCount++;
}

void os_start( void ) { 
	__ASM("SVC #0x0");
}

void os_release( void ){
	
	printf("OS release\n");
	
	os_Registers* target_regs;
	os_Registers* current_regs;
	if (control.currentTask == 0) {
		target_regs = task_queue[1];
		current_regs = task_queue[0];
		control.currentTask = 1;
	} else {
		target_regs = task_queue[0];
		current_regs = task_queue[1];
		control.currentTask = 0;
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
	/* SysTick_Config(0x00FFFFFF); */
	
	
	init_task(&t1_func, t1_stack, &t1_reg);
	init_task(&t2_func, t2_stack, &t2_reg);
	
	os_start();
	
	//	os_start();
	
	//	Trigger System Call
	Sys_Call(&t1_reg);
	
	for (;;) {
		printf("Hello world\n");
	}
}
