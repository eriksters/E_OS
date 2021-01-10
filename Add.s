						;	Put what follows in .text section
						;	The following are instructions and are readonly
						AREA 	|.text|, CODE, READONLY
						
						EXPORT	add_and_store	;	Make add_and_store a global symbol
						
						;	Procedure: add_and_store
						;	Add 2 numbers.
						;	R0 - number 1
						;	R1 - address of number 2
						;	R2 - address to store result
add_and_store			PROC
						
						LDR		r1, [r1]		;	Load values of number 2	( destination, source )
						ADD		r0, r0, r1		;	Add the 2 numbers 		( destination, source1, source2 )
						STR		r0, [r2]		;	Store the result
						BX		lr				;	Branch back to calling procedure
						
						ENDP					;	End of procedure
						
						END						;	End of file
							
							
						