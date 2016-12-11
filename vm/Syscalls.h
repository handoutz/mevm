#pragma once
#include "virtuo.h"
#include "common.h"
#include "register.h"
#include <string>
using namespace std;
class Syscalls
{
public:
	virtuo* m_ctx;
	Syscalls(virtuo* c) :m_ctx(c) {}
	void route_call(BASE_TYPE cnum);
	void _WRITEL();
	void _WRITEV();
	void _GETLINE();
	void _WRITES();
};

