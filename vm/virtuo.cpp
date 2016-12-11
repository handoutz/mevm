#include "StdAfx.h"
#include "parser.h"
#include <iostream>
#include "virtuo.h"
#include "packer.h"
using namespace std;

virtuo::virtuo(void) :m_curStrId(5050), last_cmp_result(-1)
{
	m_registers = new Register<BASE_TYPE>[N_REGISTERS];
	m_strRegisters = new Register<StrPart>[N_REGISTERS];
	for (int i = 0; i < N_REGISTERS; i++)
	{
		m_registers[i] = Register<BASE_TYPE>(ID_REGI_START + i);
		m_strRegisters[i] = Register<StrPart>(ID_STRREGI_START + i);
	}
}


virtuo::~virtuo(void)
{
}

void virtuo::dmpState()
{
	cout << endl << "Dumping Registers" << endl;
	for (int i = 0; i < N_REGISTERS; i++) {
		if (m_registers[i].m_value != 0)
			printf("Regi #%i = 0x%X\n", i, m_registers[i].m_value);
		if (!m_strRegisters[i].m_value.isempty())
			printf("strRegi #%i = [%s]\n", i, m_strRegisters[i].m_value.m_value);
	}
	cout << endl << endl << "Dumping stack" << endl;
	int n = 0;
	while (!m_stack.empty()) {
		BASE_TYPE val;
		if (stack_pop(val))
			printf("%i: 0x%X\n", n, val);
		//cout << "stack value [" << val << "]"<<endl;
	}
}
inline BASE_TYPE _nxtchar(BASE_TYPE *program, int &iPtr) {
	auto i = program[++iPtr];
	//hexDump("a", program, 1,6,iPtr);
	//printf("%08x [%s]\n", i, get_opcode_name((OPCODES)i));
	return i;
}
#define nxtchar _nxtchar(program,iPtr)

void virtuo::preparse(BASE_TYPE *program, int sz)
{
	int iPtr = -1;
	do {
		iPtr++;
		BASE_TYPE _cur = program[iPtr];
		if (_cur == INLINE_STR_START) {
			program[iPtr] = INLINE_ID_REF;
			InlineString *_is = new InlineString(m_curStrId++);
			BASE_TYPE curchr = nxtchar;
			program[iPtr] = _is->m_id;
			do {
				if (curchr != INLINE_STR_START) {
					_is->val.append(curchr);
				}
				curchr = nxtchar;
				program[iPtr] = NOP;
			} while (curchr != INLINE_STR_END);
			m_strings.push_back(_is);
		}
	} while (iPtr < sz);
	iPtr = -1;
	do {
		iPtr++;
		BASE_TYPE _cur = program[iPtr];
		PLabel* lbl = nullptr;
		if (_cur == LBL_START) {
			lbl = mklabel(iPtr, nxtchar);
		}
		if (_cur == LBL_END&&lbl != nullptr) {
			lbl->m_endLoc = iPtr;
			lbl = nullptr;
		}
	} while (iPtr < sz);
}
InlineString* virtuo::next_str(virtuo *v, BASE_TYPE *program, int &sz, int &iPtr) {
	InlineString* nxt = nullptr;
	BASE_TYPE c = nxtchar;
	if (c == INLINE_ID_REF) {
		BASE_TYPE id = nxtchar;
		if (v->findstr(id, nxt)) {
			return nxt;
		}
		else {
			return nullptr;
		}
		/*do {
		if (curchr != INLINE_STR_START) {
		r->m_value.append(curchr);
		}
		curchr = nxtchar;
		} while (curchr != INLINE_STR_END);*/
	}
	else if (c == INLINE_STR_START) {
		BASE_TYPE curchr = nxtchar;
		nxt = new InlineString(m_curStrId++);
		do {
			if (curchr != INLINE_STR_START)
				nxt->val.append(curchr);
			curchr = nxtchar;
		} while (curchr != INLINE_STR_END);
	}
	return NULL;
}
#include "Syscalls.h"
void virtuo::run(BASE_TYPE *program, int sz)
{
	preparse(program, sz);
	hexDump("Running this program", program, sz);
	printf("\n---------------------------\n");
	int iPtr = -1;
	bool was_main_run = false;
	//PLabel* curLabel = nullptr;
	Syscalls sysCalls(this);
	do {
		//iPtr++;
		BASE_TYPE _cur = nxtchar;
		switch (_cur) {
		case NOP: {
			continue;
		}
		case LOADI: {
			BASE_TYPE rnum = program[++iPtr];
			BASE_TYPE ival = program[++iPtr];
			regi(rnum)->m_value = ival;
			break;
		}
		case MOV: {
			BASE_TYPE lreg = program[++iPtr], rreg = program[++iPtr];
			//regi(rreg)->m_value = regi(lreg)->m_value;
			regi(rreg)->set(regi(lreg)->m_value);
			break;
		}
		case PUSH: {
			BASE_TYPE rnum = nxtchar;
			stack_push(rval(rnum));
			break;
		}
		case PUSHI: {
			stack_push(nxtchar);
			break;
		}
		case POP: {
			BASE_TYPE rnum = nxtchar;
			BASE_TYPE popval;
			if (stack_pop(popval))
				regi(rnum)->m_value = popval;
			break;
		}//INC, DEC, ADD, SUB, MUL, DIV
		case INC:
			regi(nxtchar)->m_value++;
			break;
		case DEC:
			regi(nxtchar)->m_value--;
			break;
		case ADD: {
			auto r0 = regi(nxtchar), r1 = regi(nxtchar);
			regi(REG_RESULT)->m_value = r0->m_value + r1->m_value;
			break;
		}
		case SUB: {
			auto r0 = regi(nxtchar), r1 = regi(nxtchar);
			regi(REG_RESULT)->m_value = r0->m_value + r1->m_value;
			break;
		}
		case MUL: {
			auto r0 = regi(nxtchar), r1 = regi(nxtchar);
			regi(REG_RESULT)->m_value = r0->m_value + r1->m_value;
			break;
		}
		case DIV: {
			auto r0 = regi(nxtchar), r1 = regi(nxtchar);
			regi(REG_RESULT)->m_value = r0->m_value + r1->m_value;
			break;
		}
		case SWP: {
			auto r0 = regi(nxtchar), r1 = regi(nxtchar);
			auto tmp = r0->m_value;
			r0->m_value = r1->m_value;
			r1->m_value = tmp;
			break;
		}
				  //a.opcode(LOADS)->strRegi(0)->s("hello, world!");
		case LOADS: {
			BASE_TYPE rnum = nxtchar;
			auto curstr = next_str(this, program, sz, iPtr);
			auto r = strregi(rnum);
			if (curstr != nullptr) {
				r->set(curstr->val);
			}
			else {
				throw NullRefException("broken tryina LOADS");
			}
			break;
		}
		case LBL_START: {
			auto lbl = findlabel(nxtchar);
			if (lbl&&lbl->m_hash == _hash_sdbm((unsigned char*)"main")
				&& !was_main_run) {
				was_main_run = true;
				m_callStack.push_stack(lbl, sz);
			}
			else {
				do {
					_cur = nxtchar;
				} while (_cur != LBL_END);
			}
			break;
		}
		case SYSCALL: {
			BASE_TYPE callnum = nxtchar;
			sysCalls.route_call(callnum);
			break;
		}
		case CALL: {
			PLabel* curLabel = findlabel(nxtchar);
			m_callStack.push_stack(curLabel, iPtr);
			//printf("calling lbl %X\n", curLabel->m_hash);
			//curLabel->setret(iPtr + 1);
			iPtr = curLabel->m_iPtr;
			break;
		}
		case RET: {
			CallStaxInfo csiCur = m_callStack.curframe(),
				csiNxt = { 0,0 };
			auto curf = csiCur;
			/*printf("popping from mtd [%s:%i] -> ", 
				parser::getlblname(curf.m_lbl->m_hash).c_str(), 
				csiCur.m_retLoc);*/
			if (m_callStack.popo(csiNxt)) {
				/*printf("[%s:%i]\n", 
					parser::getlblname(csiNxt.m_lbl->m_hash).c_str(), 
					csiNxt.m_retLoc);*/
			}
			iPtr = csiCur.m_retLoc;
			break;
		}
#define JUMPCASE(opc, CMPCODE)\
auto curLabel=findlabel(nxtchar);\
if(!(last_cmp_result&CMPCODE)){\
	break;\
}\
m_callStack.push_stack(curLabel, iPtr);\
iPtr=curLabel->m_iPtr;

		case JE: {
			JUMPCASE(JE, CMP_EQS);
			break;
		}
		case JNE: {
			JUMPCASE(JNE, CMP_NEQS);
			break;
		}
		case JGT: {
			JUMPCASE(JGT, CMP_GT);
			break;
		}
		case JLT: {
			JUMPCASE(JLT, CMP_LT);
			break;
		}
		case CMP: {
			auto _r0 = regi(nxtchar), _r1 = regi(nxtchar);
			auto r0 = _r0->get(), r1 = _r1->get();
			last_cmp_result = 0;
			if (r0 == r1)last_cmp_result |= CMP_EQS;
			if (r0 != r1)last_cmp_result |= CMP_NEQS;
			if (r0 > r1)last_cmp_result |= CMP_GT;
			if (r0 < r1)last_cmp_result |= CMP_LT;
			break;
		}
		}
	} while (iPtr < sz);
	printf("\n---------------------------\n");
}
