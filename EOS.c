#include "EOS.h"
#include "EOS_Dispatcher.h"
#include "stm32f10x.h"
#include "EOS_Scheduler.h"

#include <stdio.h>


void os_DeleteTask ( os_Registers_t * tcb ) {
	tcb = tcb + 1;
}

void os_Start ( void ) {
	__asm("SVC #0x00");
}

void os_Release ( void ) {
	__asm("SVC #0x01");
}

void os_TaskEnd ( void ) {
	__asm("SVC #0x02");
}

void os_Delay( uint32_t milliseconds ) {
	UNUSED(milliseconds);
	__asm("SVC #0x3");
}


