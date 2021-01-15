#include <stdio.h>
#include "EOS.h"
#include "stm32f10x.h"
#include "EOS_Tests.h"

//	Example ASM
int32_t add_and_store( int32_t num1, int32_t* num2_p, int32_t* result_p );

//	Clock configuration
void SystemCoreClockConfigure( void );

//	SysTick
void SysTick_Handler( void );

//	Loop for count times
void Loop( uint32_t count );

//	Release until a mutex is acquired
void acquire_mutex( os_mutex_h );

 /*******************************************
 *							Configuration        				*
 *******************************************/


/* Slightly modified version of Clock Setup from example project */
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


void SysTick_Handler( void ) {	
	os_tick();
}

void Loop( uint32_t count ) {
	for ( uint32_t i = 0; i < count; i++ ) {
		__NOP();
	}
}

void acquire_mutex( os_mutex_h mutex ) {
	while ( os_lock_mutex( mutex )) os_release();
}

 /*******************************************
 *							Program Data    						*
 *******************************************/

/* --------------		Handles		------------ */
static os_mutex_h LED_1_mutex_H;
static os_mutex_h LED_2_mutex_H;
static os_mutex_h console_mutex_H;

static os_task_h task_1_H;
static os_task_h task_2_H;
static os_task_h task_3_H;


/* -----		Task Parameter Structs		---- */
struct t1_params_t {
	uint32_t dummy_number;
};

struct t2_params_t {
	uint32_t dummy_number;
	os_mutex_t* dummy_pointer;
};

struct t3_params_t {
	uint32_t dummy_number;
};

/* ------------		Data Section		-------- */
//	Mutexes
static os_mutex_t mutex_1;
static os_mutex_t mutex_2;
static os_mutex_t mutex_3;

//	Task Control Blocks
static os_TCB_t t1_tcb;
static os_TCB_t t2_tcb;
static os_TCB_t t3_tcb;

//	Task parameters
static struct t1_params_t t1_params;
static struct t2_params_t t2_params;
static struct t3_params_t t3_params;

//	Task function declarations
void t1_func( void * params ); 
void t2_func( void * params ) __attribute__((noreturn));
void t3_func( void * params);

//	Task function definitions
void t1_func( void * params ) {
	
	struct t1_params_t* par = (struct t1_params_t*) params;
	
	acquire_mutex( console_mutex_H );
	printf("T1 Entry. Dummy number = %d\n", par->dummy_number );
	printf("T1. Delaying for 3s. Mutex still locked.\n");
	
	os_delay(3000);
	
	for ( uint32_t i = 0; i < 3; i++ ) {
		os_unlock_mutex( console_mutex_H );
		os_delay(1000);
		acquire_mutex( console_mutex_H );
		printf("Task 1. Mutex acquired. Count = %d\n", i);
	}
	
	printf("Task 1. Finished. Mutex is still locked.\n");
}

void t2_func( void * params ) {
	struct t2_params_t* par = (struct t2_params_t*) params;
	
	acquire_mutex( console_mutex_H );
	
	printf("T2. Entry. Dummy number = %d; Dummy Pointer = %p\n", par->dummy_number, (void *) par->dummy_pointer );
	printf("T2. Starting task 3\n");
	
	task_3_H = os_create_task(&t3_func, &t3_tcb, &t3_params);
	
	uint32_t count = 0;
	for ( ;; ) {
		printf("T3. Delaying for 1s, unlocking.\n");
		os_unlock_mutex( console_mutex_H );
		os_delay(1000);
		acquire_mutex( console_mutex_H );
		printf("Task 2. Mutex locked. Count = %d\n", count);
		count++;
	}
	
}

void t3_func( void * params ) {
	
	struct t1_params_t* par = (struct t1_params_t*) params;
	
	acquire_mutex( console_mutex_H );
	printf("T3. Entry. Dummy number = %d\n", par->dummy_number );
	printf("T3. Delaying for 3s, unlocking.\n");
	os_unlock_mutex( console_mutex_H );
	
	os_delay( 3000 );
	
	for ( int i = 0; i < 10; i++) {
		acquire_mutex( console_mutex_H );
		printf("T3. Locked. Delaying for 0.3s \n");
		os_delay(300);
		os_unlock_mutex( console_mutex_H );
	}
	
	acquire_mutex( console_mutex_H );
}

int main() {
	
	//	Clock setup
	
	SystemCoreClockConfigure();
	SystemCoreClockUpdate();
	
	SysTick_Config(0x0009C400);
	NVIC_SetPriority( SysTick_IRQn, 0);
	
	
	//	Tests
	uint32_t failed_data_structures_tests = os_data_structures_test();
	printf( "Data structures failed tests: %d\n", failed_data_structures_tests );
	
	uint32_t failed_modules = os_all_tests();
	printf( "Failed modules: %d\n", failed_modules );
	
	
	//	Example ASM
	int32_t num_1 = 20;
	int32_t num_2 = -30;
	int32_t result;
	int32_t result_ret = add_and_store( num_1, &num_2, &result );
	
	printf("Returned = %d, stored = %d\n", result_ret, result);
	
	//	Initialize OS
	os_init( 0 );
	
	//	Create mutexes
	LED_1_mutex_H = os_create_mutex( &mutex_1 );
	LED_2_mutex_H = os_create_mutex( &mutex_2 );
	console_mutex_H = os_create_mutex( &mutex_3 );
	
	//	Assign task parameters
	t1_params.dummy_number = 69;
	t2_params.dummy_number = 420;
	t2_params.dummy_pointer = &mutex_1;
	t3_params.dummy_number = 720;
	
	//	Create tasks
	task_1_H = os_create_task(&t1_func, &t1_tcb, &t1_params);
	task_2_H = os_create_task(&t2_func, &t2_tcb, &t2_params);
	
	//	Starting OS
	printf("Starting OS\n");
	os_start();
	
	for (;;) {
		printf("All Threads Exited \n");
	}
}
