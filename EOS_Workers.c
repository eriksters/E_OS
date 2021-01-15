#include "EOS_Workers.h"
#include "EOS_Core.h"
#include "EOS.h"

os_task_h os_exit_worker_H;
os_task_h os_wait_worker_H;

static os_TCB_t exit_worker;
static os_TCB_t wait_worker;

void os_workers_init() {
	//	Initialize OS Shutdown task
	os_arch_create_task( &os_exit_task, (uint32_t*) exit_worker.stack, &exit_worker.backed_up_registers, 0 );
	os_exit_worker_H = &exit_worker;
	os_exit_worker_H->state = OS_TASK_STATE_READY;
	
	
	//	Initialize OS Wait task
	os_arch_create_task( &os_wait_task, (uint32_t*) wait_worker.stack, &wait_worker.backed_up_registers, 0 );
	os_wait_worker_H = &wait_worker;
	os_wait_worker_H->state = OS_TASK_STATE_READY;
}

void os_exit_task( void * param ) {
	UNUSED( param );
	os_exit();
}

void os_wait_task( void * param ) {
	UNUSED( param );
	for(;;);
}





