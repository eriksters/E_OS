#ifndef EOS_DEFINES_H
#define EOS_DEFINES_H


//	OS STATES

#define OS_STATE_PRE_INIT				0x0
#define OS_STATE_INIT						0x1
#define OS_STATE_POST_INIT			0x2
#define OS_STATE_STARTING				0x3
#define OS_STATE_RUNNING				0x4
#define OS_STATE_EXIT						0x5
#define OS_STATE_DONE						0x6


//	OS TASK STATES

#define OS_TASK_STATE_RUNNING		0x0
#define OS_TASK_STATE_READY 		0x1
#define OS_TASK_STATE_BLOCKED		0x2
#define OS_TASK_STATE_ZOMBIE		0x3
#define OS_TASK_STATE_DELETED		0x4

#endif
