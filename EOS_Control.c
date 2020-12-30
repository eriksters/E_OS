#include "EOS_Control.h"

os_Control_t os_Control;

uint32_t os_ctrl_get_status( void ) {
	return (uint32_t) os_Control.status;
}

void os_ctrl_set_status_running( void ) {
	os_Control.status = running;
}

os_TCB_t* os_ctrl_get_current_task( void ) {
	return os_Control.currentTask;
}

os_Registers_t* os_ctrl_get_current_task_reg( void ) {
	return &os_Control.currentTask->backed_up_registers;
}
