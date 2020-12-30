#include <stdio.h>
#include "EOS.h"

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

void SysTick_Handler( void );
void SysTick_Handler( void ) {
	printf("In SysTick_Handler\n");
	
	os_tick();
}

void EXTI0_IRQHandler( void );
void EXTI0_IRQHandler( void ) {
	printf("EXTI 0 \n");
}

static os_mutex_t mutex_1;

static os_TCB_t t1_tcb;
void t1_func( void ) __attribute__((noreturn)); 

static os_TCB_t t2_tcb;
void t2_func( void ) __attribute__((noreturn));

static os_TCB_t t3_tcb;
void t3_func( void ) __attribute__((noreturn));

void t1_func( void ) {
	for (;;) {
		if ( !os_mutex_lock(&mutex_1) ) {
			printf("T1 Mutex Locked\n");
		} else {
			printf("T1 Mutex NOT Lock\n");
		}
		for (int j = 0; j < 10; j++) {	
			printf("t1 [locked]. Count = %d\n", j);
			for (int i = 0; i < 10000; i++) {
				__NOP();
			}
		}
		if ( !os_mutex_unlock(&mutex_1) ) {
			printf("T1 mutex UNlocked\n");
		} else {
			printf("T1 mutex NOT UNlocked\n");
		}
		for (int j = 0; j < 10; j++) {	
			printf("t1 [unlocked]. Count = %d\n", j);
			for (int i = 0; i < 10000; i++) {
				__NOP();
			}
		}
	}
}

void t2_func( void ) {
	for (;;) {
		for (int j = 0; j < 20; j++) {		
			printf("t2 func. Count = %d\n", j);
			for (int i = 0; i < 10000; i++) {
				__NOP();
			}
		}
		os_delay(2000);
	}
}

void t3_func( void ) {
	for (;;) {
		if ( !os_mutex_lock(&mutex_1) ) {
			printf("T3 Mutex Locked\n");
		} else {
			printf("T3 Mutex NOT Lock\n");
		}
		for (int j = 0; j < 5; j++) {		
			printf("t3 [locked]. Count = %d\n", j);
			for (int i = 0; i < 10000; i++) {
				__NOP();
			}
		}
		if ( !os_mutex_unlock(&mutex_1) ) {
			printf("T3 mutex UNlocked\n");
		} else {
			printf("T3 mutex NOT UNlocked\n");
		}
		for (int j = 0; j < 5; j++) {		
			printf("t3 [unlocked]. Count = %d\n", j);
			for (int i = 0; i < 10000; i++) {
				__NOP();
			}
		}
	}
}


int main() {
	
	//	Clock setup
	
	SystemCoreClockConfigure();
	SystemCoreClockUpdate();
	
	SysTick_Config(0x0009C400);
	
	os_init( 0 );
	
	os_mutex_create( &mutex_1 );
	
	printf("Creating Tasks\n");
	os_task_create(&t1_func, &t1_tcb);
	os_task_create(&t2_func, &t2_tcb);
	os_task_create(&t3_func, &t3_tcb);
	
	printf("Starting OS\n");
	os_start();
	
	for (;;) {
		printf("All Threads Exited \n");
	}
}
