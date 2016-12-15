#include <stdlib.me>
main:
	LOADI r0 @1
	LOADI r1 @1
	CALL dotest
	LOADI r0 @1
	LOADI r1 @2
	CALL dotest
	RET
dotest:
	CMP r0 r1z
	JE eqs
	JNE neqs
	RET
eqs:
	WRITE_LITERAL_STR("dey eqals", sr1)
	RET
neqs:
	WRITE_LITERAL_STR("dey not eq", sr1)
	RET