#include "stdafx.h"
#include "assembler.h"

assemblr* assemblr::s(string str)
{
	opcode(INLINE_STR_START);
	int nToPad = 4 - (str.size() % 4);
	nToPad > 0 ? str.append(nToPad, '\0') : 0;
	int n = str.size();
	for (int i = -1; i < (int)n - 1;) {
		BASE_TYPE val = pack(nxti, nxti, nxti, nxti);
		immediate(val);
		//char* ss=unpack_str4(val);
		//printf("i=%i, n=%i, nToPad=%i, origstr=[%s]  str=[%s]\n", i,n,nToPad,str.c_str(), ss);
	}
	opcode(INLINE_STR_END);
	return this;
}

assemblr* assemblr::call(char* _name)
{
	opcode(CALL)->immediate(_hash_sdbm((unsigned char*)_name));
	return this;
}

assemblr* assemblr::syscall(SYSCALLS sc)
{
	opcode(SYSCALL)->immediate(sc);
	return this;
}

assemblr* assemblr::opcode(OPCODES op)
{
	m_prog.push_back(op);
	return this;
}

assemblr* assemblr::kw(KEYWORDS kw)
{
	m_prog.push_back(kw);
	return this;
}

assemblr* assemblr::immediate(BASE_TYPE val)
{
	m_prog.push_back(val);
	return this;
}

assemblr* assemblr::regi(BASE_TYPE num)
{
	m_prog.push_back(num);
	return this;
}

assemblr* assemblr::strRegi(BASE_TYPE num)
{
	m_prog.push_back(num + ID_STRREGI_START);
	return this;
}

assemblr* assemblr::end()
{
	m_prog.push_back(ENDOP1);
	m_prog.push_back(ENDOP2);
	m_prog.push_back(ENDOP3);
	m_prog.push_back(ENDOP4);
	return this;
}

subroutine_assember* assemblr::subroutine(char* lbl) {
	auto sa = new subroutine_assember(this, lbl);
	return sa;
}

void assemblr::_subroutine_finished(subroutine_assember *sa)
{
	int sz;
	auto prog = sa->assemble(sz);
	for (auto i = 0; i < sz; i++)
		m_prog.push_back(prog[i]);
}

