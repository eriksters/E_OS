#include "stm32f10x.h"
#include <stdio.h>

#include "EOS.h"
#include "EOS_Scheduler.h"

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


static os_Registers_t t1_reg;
static os_TaskStack_t t1_stack;
void t1_func( void ) __attribute__((noreturn)); 

static os_Registers_t t2_reg;
static os_TaskStack_t t2_stack;
void t2_func( void ) __attribute__((noreturn));

static os_Registers_t t3_reg;
static os_TaskStack_t t3_stack;
void t3_func( void ) __attribute__((noreturn));

void t1_func( void ) {
	for (;;) {
		for (int j = 0; j < 10; j++) {		//	Execute 10 times and then release
			printf("t1 func. Count = %d\n", j);
			for (int i = 0; i < 10000; i++) {
				__NOP();
			}
		}
		os_Release();
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
		os_Release();
	}
}

void t3_func( void ) {
	for (;;) {
		for (int j = 0; j < 5; j++) {		//	Execute 5 times and then release
			printf("t3 func. Count = %d\n", j);
			for (int i = 0; i < 10000; i++) {
				__NOP();
			}
		}
		os_Release();
	}
}

int main() {
	
	//	Clock setup
	
	SystemCoreClockConfigure();
	/* SysTick_Config(0x00FFFFFF); */
	
	os_Queue_init();
	
	printf("Creating Tasks\n");
	os_CreateTask(&t1_func, &t1_stack, &t1_reg);
	os_CreateTask(&t2_func, &t2_stack, &t2_reg);
	os_CreateTask(&t3_func, &t3_stack, &t3_reg);
	
	printf("Starting OS\n");
	os_Start();
	
	for (;;) {
		printf("All Threads Exited \n");
	}
}
