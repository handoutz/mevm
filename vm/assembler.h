#pragma once
#define ENDOP1 0xDE
#define ENDOP2 0xAD
#define ENDOP3 0xBE
#define ENDOP4 0xEF
#include "common.h"

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
    assemblr* call(const char* _name) {
        return call((char*)_name);
    }
    assemblr* lblname(char* _name) {
        immediate(_hash_sdbm((unsigned char*)_name));
        return this;
    }
    assemblr* lblname(const char* _name) { return lblname((char*)_name); }
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
    subroutine_assember* subroutine(const char* lbl) {
        return subroutine((char*)lbl);
    }
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