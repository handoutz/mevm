#pragma once
#include "assembler.h"
#include "parser.h"
struct TestProgram {
	BASE_TYPE* program;
	int sz;
	char* name;
};
inline TestProgram* test_prog_assembleme() {
	TestProgram *t=new TestProgram;
	assemblr a;
	parser p;
	int sz = 0;
	t->program = p.parse_file("assemble.me", t->sz);
	t->name = "assemble.me";
	return t;
}

inline TestProgram *test_prog_conditionals() {
	TestProgram *t = new TestProgram;
	assemblr a;
	parser p;
	int sz = 0;
	t->program = p.parse_file("conditionals.me", t->sz);
	t->name = "conditionals.me";
	return t;
}
inline TestProgram *test_prog_hello() {
	TestProgram *t = new TestProgram;
	assemblr a;
	t->program = a.
		subroutine("main")->call("lol")->opcode(RET)->finish()->
		subroutine("lol")->
		opcode(LOADS)->strRegi(1)->s("What's your name?")->
		opcode(PUSHI)->strRegi(1)->
		opcode(PUSHI)->strRegi(0)->
		syscall(GETLINE)->
		opcode(LOADS)->strRegi(1)->s("Hello, ")->
		opcode(PUSHI)->strRegi(1)->
		syscall(WRITES)->
		opcode(PUSHI)->strRegi(0)->
		syscall(WRITES)->
		opcode(LOADS)->strRegi(1)->s(", how are you?")->
		opcode(PUSHI)->strRegi(1)->
		syscall(WRITES)->
		opcode(RET)->finish()->
		assemble(t->sz);
	t->name = "assemble.me C++ Literal";
	return t;
}
