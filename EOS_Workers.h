#ifndef EOS_WORKERS_H
#define EOS_WORKERS_H

#include "EOS_Core.h"

extern os_TCB_t os_exit_worker_H;
void os_exit_task( void * );

extern os_TCB_t os_wait_worker_H;
void os_wait_task( void * ) __attribute__((noreturn));



#endif
