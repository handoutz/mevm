#include "stdafx.h"
#include "assembler.h"

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

