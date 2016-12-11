#include "stdafx.h"
#include "parser.h"
#include "StringAlgos.h"

parser::parser()
{
}


parser::~parser()
{
}

std::map<BASE_TYPE, std::string> parser::m_lblmap;

void parser::pushlbl(BASE_TYPE hsh, string na)
{
	m_lblmap[hsh] = na;
}

std::string parser::getlblname(BASE_TYPE hsh)
{
	auto res = m_lblmap.find(hsh);
	if (res != m_lblmap.end())
		return res->second;
	return "UNKNOWN";
}

inline BASE_TYPE rnum(string c) {
	if (c.size() < 1)return -1;
	if (c[0] == 'r')
		return atoi(&c[1]);
	if (c[0] == 's')
		return atoi(&c[2]);
	return -1;
}
inline BASE_TYPE get_immediate(string c) {
	if (c.size() < 1)return -1;
	if (c[0] == '@')
		return atoi(&c[1]);
	return -1;
}
inline BASE_TYPE get_sysall(string c) {
	if (c.size() < 1)return -1;
	if (c[0] == '@') {
		string namen = c.substr(1);
		if (namen == "WRITEL")return WRITEL;
		else if (namen == "WRITEVAL")return WRITEVAL;
	}
	return -1;
}
inline assemblr* getptr(assemblr* mainr, subroutine_assember* cursub) {
	return cursub != nullptr ? cursub : mainr;
}
#define PARSE_CASE_START(opc) if(frst==#opc)
#define PARSE_CASE(opc) else PARSE_CASE_START(opc)
BASE_TYPE * parser::parse(const char* texts, int &sz)
{
	istringstream ins;
	ins.str(texts);
	assemblr a;
	assemblr* cura = &a;
	subroutine_assember* cursub;
	for (string _line; getline(ins, _line); ) {
		auto stmtsplit = str::tokenize(_line.c_str(), ";;");
		for (auto iSplit = 0; iSplit < stmtsplit.size(); iSplit++) {
			string line = stmtsplit.at(iSplit);
			if (line.size() <= 1 || line[0] == ';' || line[1] == '#')continue;
			string fullline(line);
			int i = 0;
			while (i < line.size() && (line[i] == ' ' || line[i] == '\t' || line[i] == '\0' || line[i] == '\r' || line[i] == '\n')) {
				i++;
			}
			line = line.substr(i);
			fullline = fullline.substr(i);
			auto split = str::tokenize(line.c_str(), " \t");
			if (split.size() == 0)continue;
			auto frst = split[0];
			if (frst[frst.size() - 1] == ':') {
				frst = frst.substr(0, frst.size() - 1);
				pushlbl(_hash_sdbm((unsigned char*)frst.c_str()), frst);
				//printf("frst=%s\n", frst.c_str());
				cursub = a.subroutine(frst.c_str());
			}
			PARSE_CASE(RET) {
				if (cursub != nullptr)cursub->opcode(RET)->finish();
				cursub = nullptr;
			}
			PARSE_CASE(LOADI) {
				auto reg = rnum(split[1]), imm = get_immediate(split[2]);
				getptr(cura, cursub)->opcode(LOADI)->regi(reg)->immediate(imm);
			}
			PARSE_CASE(CALL) {
				getptr(cura, cursub)->call(split[1].c_str());
			}
			PARSE_CASE(LOADS) {
				auto reg = rnum(split[1]);
				auto iFirstQt = str::indexof(line, "\"") + 1;
				auto iLastQt = str::indexof(line, "\"", iFirstQt + 1);
				auto qtstr = fullline.substr(iFirstQt, iLastQt - iFirstQt);
				getptr(cura, cursub)->opcode(LOADS)->strRegi(reg)->s(qtstr);
			}
			PARSE_CASE(PUSH) {
				auto reg = rnum(split[1]);
				if (split[1].find("sr") != string::npos)
					getptr(cura, cursub)->opcode(PUSH)->strRegi(reg);
				else
					getptr(cura, cursub)->opcode(PUSH)->regi(reg);
			}
			PARSE_CASE(PUSHI) {
				auto reg = get_immediate(split[1]);
				if (split[1].find("sr") != string::npos) {
					reg = rnum(split[1]);
					getptr(cura, cursub)->opcode(PUSHI)->strRegi(reg);
				}
				else if (split[1].find("r") != string::npos) {
					reg = rnum(split[1]);
					getptr(cura, cursub)->opcode(PUSHI)->regi(reg);
				}
				else {
					getptr(cura, cursub)->opcode(PUSHI)->immediate(reg);
				}
			}
			PARSE_CASE(SYSCALL) {
				//auto syscallnum = get_sysall(split[1]);
				auto syscallnum = get_syscall_val(split[1]);
				getptr(cura, cursub)->syscall((SYSCALLS)syscallnum);
			}
			PARSE_CASE(MOV) {
				getptr(cura, cursub)->opcode(MOV)->
					regi(rnum(split[1]))->regi(rnum(split[2]));
			}
			PARSE_CASE(INC) {
				getptr(cura, cursub)->opcode(INC)->regi(rnum(split[1]));
			}
			PARSE_CASE(DEC) {
				getptr(cura, cursub)->opcode(DEC)->regi(rnum(split[1]));
			}
#define arithmetic_op_case(opc) PARSE_CASE(opc){getptr(cura,cursub)->opcode(opc)->regi(rnum(split[1]))->regi(rnum(split[2]));}
			arithmetic_op_case(ADD)
				arithmetic_op_case(SUB)
				arithmetic_op_case(MUL)
				arithmetic_op_case(DIV)
				PARSE_CASE(SWP) {
				getptr(cura, cursub)->opcode(DEC)->regi(rnum(split[1]));
			}
			PARSE_CASE(POP) {
				getptr(cura, cursub)->opcode(POP)->regi(rnum(split[1]));
			}
			PARSE_CASE(CMP) {
				getptr(cura, cursub)->opcode(CMP)->
					regi(rnum(split[1]))->regi(rnum(split[2]));
			}
#define jmpcase(opc) PARSE_CASE(opc){\
getptr(cura, cursub)->opcode(opc)->lblname(split[1].c_str());}
			jmpcase(JE)
				jmpcase(JNE)
				jmpcase(JLT)
				jmpcase(JGT)
		}
	}
	if (cursub != nullptr) {
		cursub->opcode(RET);
		cursub->finish();
	}
	return a.assemble(sz);
}
