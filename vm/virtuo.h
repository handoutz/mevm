#pragma once
#include "packer.h"
#include <vector>
#include "assembler.h"
#include "trace.h"
#include "VMException.h"
#include "register.h"
#include <string>
using namespace std;

#define N_REGISTERS 255
#include <stack>
class PLabel {
public:
	BASE_TYPE m_hash;
	int m_iPtr;
	int m_retLoc;
	PLabel() : m_hash(0), m_iPtr(0), m_retLoc(0), m_endLoc(0) {}
	PLabel(int iptr, BASE_TYPE hsh) :m_iPtr(iptr), m_hash(hsh), m_retLoc(0), m_endLoc(0) {}

	void setret(int loc) { m_retLoc = loc; }
	int retloc() const { return m_retLoc; }
	int m_endLoc;
};
class virtuo
{
public:
	Register<BASE_TYPE> *m_registers;
	Register<StrPart> *m_strRegisters;
	std::stack<BASE_TYPE> m_stack;
	std::vector<InlineString*> m_strings;
	std::vector<PLabel*> m_labels;

	virtuo(void);
	~virtuo(void);

	PLabel* mklabel(int ptr, BASE_TYPE hsh) {
		auto lbl = new PLabel(ptr, hsh);
		m_labels.push_back(lbl);
		return lbl;
	}
	PLabel* findlabel(BASE_TYPE hsh) {
		for (size_t i = 0; i < m_labels.size(); i++) {
			auto l = m_labels.at(i);
			if (l->m_hash == hsh)
				return l;
		}
		return nullptr;
	}

	bool findstr(BASE_TYPE id, InlineString* &res) {
		for (size_t i = 0; i < m_strings.size(); i++) {
			auto rf = m_strings.at(i);
			if (rf->m_id == id) {
				res = rf;
				return true;
			}
		}
		return false;
	}

	Register<BASE_TYPE>* regi(BASE_TYPE num) {
		num -= ID_REGI_START;
		if (num >= N_REGISTERS)return nullptr;
		return &m_registers[num];
	}
	Register<StrPart>* strregi(BASE_TYPE num) {
		num -= ID_STRREGI_START;
		if (num >= N_REGISTERS)return nullptr;
		return &m_strRegisters[num];
	}
	BASE_TYPE rval(BASE_TYPE num) {
		auto reg = regi(num);
		if (reg == nullptr)return -1;
		return reg->m_value;
	}
	void stack_push(BASE_TYPE val) {
		m_stack.push(val);
	}
	bool stack_pop(BASE_TYPE& val) {
		if (m_stack.empty())return false;
		val = m_stack.top();
		m_stack.pop();
		return true;
	}
	void dmpState();
	void preparse(BASE_TYPE *program, int sz);
	void run(BASE_TYPE *program, int sz);
};

