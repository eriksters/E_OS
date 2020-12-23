#include "EOS_Data.h"

os_Control_t os_Control;

uint32_t os_GetStatus( void ) {
	return (uint32_t) os_Control.status;
}

void os_SetStarted( void ) {
	os_Control.status = running;
}

uint32_t* os_getCurrentTask( void ) {
	return (uint32_t*) os_Control.currentTask;
}
