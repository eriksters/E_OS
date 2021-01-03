#include "EOS_Workers.h"


os_TCB_t os_exit_worker_H;
os_TCB_t os_wait_worker_H;

extern void os_exit( void );


void os_exit_task( void * param ) {
	UNUSED( param );
	os_exit();
}

void os_wait_task( void * param ) {
	UNUSED( param );
	for(;;);
}
