#pragma once
#include "packer.h"
#include <vector>
#include "assembler.h"
#include "trace.h"
#include "VMException.h"
#include <string>
using namespace std;

template<typename T>
class Register {
public:
	BASE_TYPE m_id;
	T m_value;
	Register(void) :m_id(0), m_value(0) {}
	Register(BASE_TYPE _id) :m_id(_id), m_value(0) {}

	void set(T nval) { m_value = nval; }
	void set(Register<T>* o) { m_value = o->m_value; }
	T get() const { return m_value; }
};
class StrPart {
public:
	char* m_value;
	StrPart(void) :m_value("") {}
	StrPart(int i) {
		m_value = unpack_str4(i);
	}
	void operator()(char* otherVal) {
		m_value = otherVal;
	}
	bool isempty() const {
		return len() == 0;
	}

	size_t len() const {
		return strlen(m_value);
	}

	void append(char c) {

	}
	void append(char* c) {
		int n = strlen(c),
			curlen = len(),
			newlen = curlen + n + 1;
		//m_value=(char*)_recalloc(m_value, newlen, sizeof(char));
		char* newval = (char*)calloc(newlen, sizeof(char));
		if (newval == nullptr) {
			throw new NullRefException("lol");
		}
		memcpy(newval, m_value, curlen);
		//TRACE("n=%i, curlen=%i, m_value[curlen]=%x", n, curlen, m_value[curlen-1]);
		int i = 0;
		for (i = curlen; i < newlen; i++) {
			newval[i] = c[i - curlen];
		}
		newval[newlen] = '\0';
		m_value = newval;
		//TRACE("m_value=[%s]", m_value);
	}
	void append(BASE_TYPE i) {
		append(unpack_str4(i));
	}
	char* str() const {
		return m_value;
	}
};
class InlineString {
public:
	BASE_TYPE m_id;
	StrPart val;
	InlineString() :m_id(50) {}
	InlineString(BASE_TYPE id) :m_id(id) {}
	InlineString(const InlineString& i) :
		m_id(i.m_id), val(i.val) {}
};