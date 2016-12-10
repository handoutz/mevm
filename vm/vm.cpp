// vm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "virtuo.h"
#include "VMException.h"

int _tmain(int argc, _TCHAR* argv[])
{
	try {
		assemblr a;
		int sz = 0;
		BASE_TYPE *program = a.
			opcode(LOADI)->regi(0)->immediate(sizeof(unsigned long))->
			opcode(LOADI)->regi(1)->immediate(sizeof(BASE_TYPE))->
			subroutine("lol")->
			opcode(LOADS)->strRegi(2)->s("Hello, subroutine world!")->
			opcode(PUSHI)->strRegi(2)->
			syscall(WRITEL)->
			opcode(RET)->finish()->

			call("lol")->
			//opcode(MOV)->regi(1)->regi(3)->
			//opcode(PUSH)->regi(1)->
			//opcode(POP)->regi(5)->opcode(PUSHI)->immediate(0xDEADBEEF)->
			//opcode(INC)->regi(1)->
			assemble(sz);
		/*int sz = 0;
		BASE_TYPE *program = a.
			opcode(LOADS)->strRegi(0)->s("hello, world!")->
			opcode(PUSHI)->strRegi(0)->syscall(WRITEL)->assemble(sz);*/

			/*BASE_TYPE *program = a.opcode(LOADI)->regi(0)->immediate(5)->
				opcode(LOADI)->regi(1)->immediate(10)->
				opcode(ADD)->regi(0)->regi(1)->
				opcode(PUSHI)->immediate(REG_RESULT)->
				syscall(WRITEVAL)->assemble(sz);*/
		virtuo v;
		v.run(program, sz);
		v.dmpState();
	}
	catch (std::exception &e) {
		printf("Error!: %s\n", e.what());
	}
	catch (...) {
		printf("Error!: unknown");
	}
	return 0;
}

