#include "EOS_Control.h"

os_Control_t os_Control;

os_state_t os_ctrl_get_state( void ) {
	return os_Control.state;
}

void os_ctrl_set_task_state_running( void ) {
	os_Control.currentTask->state = OS_TASK_STATE_RUNNING;
}

//	TODO: set to any state
void os_ctrl_set_state_running( void ) {
	os_Control.state = OS_STATE_RUNNING;
}

os_TCB_t* os_ctrl_get_current_task( void ) {
	return os_Control.currentTask;
}

os_Registers_t* os_ctrl_get_current_task_reg( void ) {
	return &os_Control.currentTask->backed_up_registers;
}
