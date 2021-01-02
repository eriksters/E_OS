#ifndef EOS_ASM_H
#define EOS_ASM_H 

/* Save registers that are not stacked during exception entry to currently running task's backed up registers data structure.
 * Implemented in EOS_ASM.s
*/
void os_reg_save( void );


/* Restore registers that are not stacked during exception entry to currently running task's backed up registers data structure.
 * Implemented in EOS_ASM.s
*/
void os_reg_restore( void );

#endif
