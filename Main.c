#include <stdio.h>
#include "EOS.h"
#include "stm32f10x.h"

void SystemCoreClockConfigure( void );
void SysTick_Handler( void );

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

 /*******************************************
 *							Program Stuff    						*					//	TODO Name
 *******************************************/

/* --------------		Handles		------------ */
static os_mutex_h mutex_1_H;
static os_mutex_h mutex_2_H;

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
	printf("T1 enter. Dummy number = %d\n", par->dummy_number );
	
	for ( uint32_t x = 1; x < 20; x++ ) {
		if ( !os_lock_mutex(&mutex_1) ) {
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
		if ( !os_unlock_mutex(&mutex_1) ) {
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
	printf("-------	T1 Delay --------------\n");
	
	os_delay(1000);
	
	printf("-------------------------T1 done----------------------\n");
}

void t2_func( void * params ) {
	
	struct t2_params_t* par = (struct t2_params_t*) params;
	printf("T2 enter. Dummy number = %d; Dummy Pointer = %p\n", par->dummy_number, (void *) par->dummy_pointer );
	
	t3_params.dummy_number = 720;
	task_3_H = os_create_task(&t3_func, &t3_tcb, &t3_params);
	
	os_lock_mutex( &mutex_2 );
	
	for (;;) {
		for (int j = 0; j < 20; j++) {		
			printf("t2 func. Count = %d\n", j);
			for (int i = 0; i < 10000; i++) {
				__NOP();
			}
		}
		os_delay(200);
        
		printf("----------------- T2, deleting -------------------\n");
		os_delete_task( 0 );
		
	}
}

void t3_func( void * params ) {
	
	struct t1_params_t* par = (struct t1_params_t*) params;
	printf("T3 enter. Dummy number = %d\n", par->dummy_number );
	
	for ( uint32_t x = 0; x < 10; x++ ) {
		if ( !os_lock_mutex(&mutex_1) ) {
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
		if ( !os_unlock_mutex(&mutex_1) ) {
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
	printf("----------------------T3 3s Delay -------------\n");
	os_delay( 3000 );
	printf("--------------------- T3 Done -----------------\n");
}

//	Example ASM
int32_t add_and_store( int32_t num1, int32_t* num2_p, int32_t* result_p );

int main() {
	
	//	Clock setup
	
	SystemCoreClockConfigure();
	SystemCoreClockUpdate();
	
	SysTick_Config(0x0009C400);
	
	//	Example ASM
	int32_t num_1 = 20;
	int32_t num_2 = -30;
	int32_t result;
	int32_t result_ret = add_and_store( num_1, &num_2, &result );
	
	printf("Returned = %d, stored = %d\n", result_ret, result);
	
	os_init( 0 );
	
	mutex_1_H = os_create_mutex( &mutex_1 );
	mutex_2_H = os_create_mutex( &mutex_2 );
	
	printf("Creating Tasks\n");
	
	t1_params.dummy_number = 69;
	
	
	task_1_H = os_create_task(&t1_func, &t1_tcb, &t1_params);
	
	
	t2_params.dummy_number = 420;
	t2_params.dummy_pointer = &mutex_1;
	task_2_H = os_create_task(&t2_func, &t2_tcb, &t2_params);
	
	printf("Starting OS\n");
	os_start();
	
	for (;;) {
		printf("All Threads Exited \n");
	}
}
