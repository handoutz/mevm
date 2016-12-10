#pragma once
#define ENDOP1 0xDE
#define ENDOP2 0xAD
#define ENDOP3 0xBE
#define ENDOP4 0xEF
//terms: iv: immediate value
typedef enum {
	NOP,			//no operation
	MOV,			//value from r0 goes into r1	ex: MOV r0 r1
	LOADI,			//loads an iv into r#			ex: LOADI r0 1337
	LOADS,			//loads a str into sr#			ex: LOADS r0 HELLO

	INC, DEC, ADD, SUB, MUL, DIV,
	//increment r#, decrement r#, 
	//adds/subtracts/multiplies/divides r# r# into REG_RESULT
	//ex: ADD r0 r1      (REG_RESULT is now r0+r1)

	SWP,			//swaps two registers values

	POP,			//pops the stack into r#		ex: POP r#
	PUSH,			//push a register value;		ex: PUSH r#
	PUSHI,			//push an immediate to r#		ex: PUSH 0

	SYSCALL,
	CALL,			//calls a label with name		ex: CALL lblname
	RET,			//returns to the calling funk	ex: RET

	INLINE_STR_START, INLINE_STR_END, INLINE_ID_REF,

	END
} OPCODES;

typedef enum {
	WRITEVAL, WRITEL
} SYSCALLS;

typedef enum {
	REG_RETVAL = 50, REG_ACCUMULATE, REG_RESULT
} SPECIAL_REGISTERS;

typedef enum {
	LBL_START=END, LBL_END
} KEYWORDS;

#include <stdint.h>

#define BASE_TYPE uint32_t
#define ID_REGI_START 0
#define ID_STRREGI_START 505

#include "packer.h"
#include <vector>
#include "trace.h"
#include "VMException.h"
#include <string>
#define nxti (++i < n ? str[i] : 0)
using namespace std;
class subroutine_assember;
class assemblr {
public:
	std::vector<BASE_TYPE> m_prog;

	assemblr* s(string str);
	assemblr* call(char* _name);
	assemblr* syscall(SYSCALLS sc);
	assemblr* opcode(OPCODES op);
	assemblr* kw(KEYWORDS kw);
	assemblr* immediate(BASE_TYPE val);
	assemblr* regi(BASE_TYPE num);
	assemblr* strRegi(BASE_TYPE num);
	assemblr* end();
	virtual assemblr* finish() { 
		return this; 
	}
	subroutine_assember* subroutine(char* lbl);
	void _subroutine_finished(subroutine_assember *sa);
	BASE_TYPE* assemble(int &n) {
		n = m_prog.size();
		BASE_TYPE *cc = new BASE_TYPE[n];
		for (int i = 0; i < n; i++)
			cc[i] = m_prog.at(i);
		return cc;
	}
};
class subroutine_assember : public assemblr {
public:
	assemblr *m_ctx;
	char *m_lbl;
	subroutine_assember(assemblr *ctx, char* lbl) :m_ctx(ctx), m_lbl(lbl) {
		kw(LBL_START)->immediate(_hash_sdbm((unsigned char*)lbl));
	}
	assemblr* finish() {
		kw(LBL_END);
		m_ctx->_subroutine_finished(this);
		return m_ctx;
	}
};