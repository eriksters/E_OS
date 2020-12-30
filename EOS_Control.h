#ifndef EOS_CONTROL_H
#define EOS_CONTROL_H

#include "EOS_Core.h"

extern os_Control_t os_Control;

os_TCB_t* os_ctrl_get_current_task( void );
os_Registers_t* os_ctrl_get_current_task_reg( void );
uint32_t os_ctrl_get_status( void );
void os_ctrl_set_status_started( void );

#endif
