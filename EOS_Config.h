#ifndef EOS_CONFIG_H
#define EOS_CONFIG_H


/*	Maximum amount of tasks
 *	Impacts size of OS data structures
*/
#define OS_MAX_TASK_COUNT 					10

/*	Maximum amount of mutexes
 *	Impacts size of OS data structures
*/
#define OS_MAX_MUTEX_COUNT					10

/*	Size of each task's stack
 *	Must be divisible by 4
*/
#define OS_TASK_STACK_SIZE					0x200


/*	Amount of os_tick calls for os to call scheduler for a task switch
*/
#define OS_TASK_SWITCH_TICK_COUNT		1


#endif
