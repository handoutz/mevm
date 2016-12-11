#include "StdAfx.h"
#include <iostream>
#include "virtuo.h"
#include "packer.h"
using namespace std;

virtuo::virtuo(void)
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

#define nxtchar program[++iPtr]
void virtuo::preparse(BASE_TYPE *program, int sz)
{
	int iPtr = -1, curStrId = 5;
	do {
		iPtr++;
		BASE_TYPE _cur = program[iPtr];
		if (_cur == INLINE_STR_START) {
			program[iPtr] = INLINE_ID_REF;
			InlineString *_is = new InlineString(curStrId++);
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
inline InlineString* next_str(virtuo *v, BASE_TYPE *program, int &sz, int &iPtr) {
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
	return NULL;
}
void virtuo::run(BASE_TYPE *program, int sz)
{
	hexDump("Running this program", program, sz);
	printf("\n---------------------------\n");
	preparse(program, sz);
	int iPtr = -1;
	bool was_main_run = false;
	PLabel* curLabel = nullptr;
	do {
		iPtr++;
		BASE_TYPE _cur = program[iPtr];
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
				&&!was_main_run) {
				was_main_run = true;
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
			BASE_TYPE rnum = 0;
			switch (callnum) {
			case WRITEL:
				if (stack_pop(rnum)) {
					printf("WRITEL: %s\n", strregi(rnum)->m_value.str());
				}
				break;
			case WRITEVAL:
				if (stack_pop(rnum)) {
					printf("R#%i = %i", rnum, regi(rnum)->m_value);
				}
				break;
			}
			break;
		}
		case CALL: {
			curLabel = findlabel(nxtchar);
			//printf("calling lbl %X\n", curLabel->m_hash);
			curLabel->setret(iPtr + 1);
			iPtr = curLabel->m_iPtr;
			break;
		}
		case RET: {
			if (curLabel == nullptr) {
			}
			else {
				iPtr = curLabel->retloc();
				curLabel->setret(0);
				curLabel = nullptr;
			}

			break;
		}
		}
	} while (iPtr < sz);
	printf("\n---------------------------\n");
}
