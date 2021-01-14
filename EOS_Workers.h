#ifndef EOS_WORKERS_H
#define EOS_WORKERS_H

#include "EOS_Tasks.h"

void os_workers_init( void );

extern os_task_h os_exit_worker_H;
void os_exit_task( void * );

extern os_task_h os_wait_worker_H;
void os_wait_task( void * ) __attribute__((noreturn));



#endif
