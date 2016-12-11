#define reg0 r0
#include <stdlib.me>

main:
	CALL lol
	RET
lol:
	LOADS sr1 "What is your name? "
	PUSHI sr1
	PUSHI sr0
	SYSCALL @GETLINE
	
	LOADS sr1 "Hello, "
	PUSHI sr1
	SYSCALL @WRITES

	PUSHI sr0
	SYSCALL @WRITES
	
	LOADS sr4 ", how are you?"
	PUSHI sr4
	SYSCALL @WRITES
	RET