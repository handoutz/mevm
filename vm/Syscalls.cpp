 #include "stdafx.h"
#include "Syscalls.h"

void Syscalls::route_call(BASE_TYPE cnum)
{
	switch (cnum) {
	case WRITEL:
		_WRITEL();
		break;
	case WRITES:
		_WRITES();
		break;
	case WRITEVAL:
		_WRITEV();
		break;
	case GETLINE:
		_GETLINE();
		break;
	}
}

void Syscalls::_WRITEL()
{
	BASE_TYPE rnum = 0;
	if (m_ctx->stack_pop(rnum)) {
		printf("WRITEL: %s\n", m_ctx->strregi(rnum)->m_value.str());
	}
}

void Syscalls::_WRITEV()
{
	BASE_TYPE rnum = 0;
	if (m_ctx->stack_pop(rnum)) {
		printf("R#%i = %i", rnum, m_ctx->regi(rnum)->m_value);
	}
}

void Syscalls::_GETLINE()
{
	BASE_TYPE rnum = 0;
	if (m_ctx->stack_pop(rnum)) {
		BASE_TYPE srnum = 0;//the prompt
		string txts;
		if (m_ctx->stack_pop(srnum))
			txts = string(m_ctx->strregi(srnum)->m_value.m_value);
		else
			txts = "Input: ";
		string inputs;
		std::cout << txts;
		std::cin >> inputs;
		StrPart nsp;
		nsp.append((char*)inputs.c_str());
		m_ctx->strregi(rnum)->set(nsp);
	}
}

void Syscalls::_WRITES()
{
	BASE_TYPE rnum = 0;
	if (m_ctx->stack_pop(rnum)) {
		printf("%s", m_ctx->strregi(rnum)->m_value.str());
	}
}
