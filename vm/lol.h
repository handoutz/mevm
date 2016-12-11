#pragma once
/*[[[cog
import cog


class NameDesc:
    name = ""
    desc = ""
    val = 0

    def __init__(self, n, d, v, ex):
        self.name = n
        self.desc = d
        self.val = v
        self.example = ex
i = 0
ndlst = []


def _out_opc(name, desc, i, ex=""):
    cog.outl("%s=%i,\t\t\t//%s\t%s" % (name.upper(), i, desc, ex))


def opc(name, desc, ex=""):
    global i
    global ndlst
    ndlst.append(NameDesc(name, desc, i, ex))
    i += 1


def opcs(names, desc, ex=""):
    for o in names:
        opc(o, desc, ex)

def dooutputs():
    global ndlst
    cog.outl("typedef enum {")
    for nd in ndlst:
        _out_opc(nd.name,nd.desc,nd.val,nd.example)
    cog.outl("} OPCODES;")

    cog.outl("inline char* get_opcode_name(OPCODES oc){")
    inum=0
    for nd in ndlst:
        cog.outl("%s(oc==%s)return \"%s\";"%("if" if inum==0 else "else if", nd.val, nd.name))
        inum+=1
    cog.outl("else return \"UNKNOWN\";}")

    cog.outl("inline int get_n_params(OPCODES oc){")
    inum=0
    for nd in ndlst:
        cog.outl("%s(oc==%s)return %i;"%("if" if inum==0 else "else if", nd.val, len(nd.example.split(' '))-1))
        inum+=1
    cog.outl("else return 0;}")
    cog.out("//list of opcodes: [")
    for nd in ndlst:
        cog.out("%s, "%nd.name)
    cog.outl("]")

opc("NOP", "no operation")
opc("MOV", "value from r0->r1", "MOV r0 r1")
opc("LOADI", "loads iv into r#", "LOADI r0 r1")
opc("LOADS", "loads str into sr#", "LOADS sr0 \"HELLO\"")
opcs(["INC", "DEC", "ADD", "SUB", "MUL", "DIV"],
     "increment r#, decrement r#, adds/subtr/mults/divides r# r# into REG_RESULT", "ADD r0 r1")
opc("SWP", "swaps two register's values", "SWP r0 r1")

opc("POP", "pops the stack into r#", "POP r#")
opc("PUSH", "push a register value;", "PUSH r#")
opc("PUSHI", "push an immediate to r#", "PUSH 0")

opc("SYSCALL", "does a syscall", "ex: SYSCALL #WRITEL")
opc("CALL", "calls a label with name", "CALL lblname")
opc("RET", "returns to the calling funk", "RET")
opc("INLINE_STR_START", "INTERNAL")
opc("INLINE_STR_END", "INTERNAL")
opc("INLINE_ID_REF", "INTERNAL")
opc("END", "the end")
dooutputs()

class _sysCall:
	name=""
	desc=""
	example=""
	def __init__(self, n, d, ex):
		self.name = n
		self.desc = d
		self.example = ex
cog.outl("typedef enum{")
for sc in [
	_sysCall("WRITEVAL", "Writes a register value to screen", "@WRITEVAL r0"),
	_sysCall("WRITEL","Writes a line to screen", "@WRITEL sr0"),
	_sysCall("GETLINE", "Gets a line from stdinput", "@GETLINE sr0")
]:
	cog.outl("%s,\t\t//%s\t%s" % (sc.name, sc.desc, sc.example))
cog.outl("} SYSCALLS;");
]]]*/
typedef enum {
NOP=0,			//no operation	
MOV=1,			//value from r0->r1	MOV r0 r1
LOADI=2,			//loads iv into r#	LOADI r0 r1
LOADS=3,			//loads str into sr#	LOADS sr0 "HELLO"
INC=4,			//increment r#, decrement r#, adds/subtr/mults/divides r# r# into REG_RESULT	ADD r0 r1
DEC=5,			//increment r#, decrement r#, adds/subtr/mults/divides r# r# into REG_RESULT	ADD r0 r1
ADD=6,			//increment r#, decrement r#, adds/subtr/mults/divides r# r# into REG_RESULT	ADD r0 r1
SUB=7,			//increment r#, decrement r#, adds/subtr/mults/divides r# r# into REG_RESULT	ADD r0 r1
MUL=8,			//increment r#, decrement r#, adds/subtr/mults/divides r# r# into REG_RESULT	ADD r0 r1
DIV=9,			//increment r#, decrement r#, adds/subtr/mults/divides r# r# into REG_RESULT	ADD r0 r1
SWP=10,			//swaps two register's values	SWP r0 r1
POP=11,			//pops the stack into r#	POP r#
PUSH=12,			//push a register value;	PUSH r#
PUSHI=13,			//push an immediate to r#	PUSH 0
SYSCALL=14,			//does a syscall	ex: SYSCALL #WRITEL
CALL=15,			//calls a label with name	CALL lblname
RET=16,			//returns to the calling funk	RET
INLINE_STR_START=17,			//INTERNAL	
INLINE_STR_END=18,			//INTERNAL	
INLINE_ID_REF=19,			//INTERNAL	
END=20,			//the end	
} OPCODES;
inline char* get_opcode_name(OPCODES oc){
if(oc==0)return "NOP";
else if(oc==1)return "MOV";
else if(oc==2)return "LOADI";
else if(oc==3)return "LOADS";
else if(oc==4)return "INC";
else if(oc==5)return "DEC";
else if(oc==6)return "ADD";
else if(oc==7)return "SUB";
else if(oc==8)return "MUL";
else if(oc==9)return "DIV";
else if(oc==10)return "SWP";
else if(oc==11)return "POP";
else if(oc==12)return "PUSH";
else if(oc==13)return "PUSHI";
else if(oc==14)return "SYSCALL";
else if(oc==15)return "CALL";
else if(oc==16)return "RET";
else if(oc==17)return "INLINE_STR_START";
else if(oc==18)return "INLINE_STR_END";
else if(oc==19)return "INLINE_ID_REF";
else if(oc==20)return "END";
else return "UNKNOWN";}
inline int get_n_params(OPCODES oc){
if(oc==0)return 0;
else if(oc==1)return 2;
else if(oc==2)return 2;
else if(oc==3)return 2;
else if(oc==4)return 2;
else if(oc==5)return 2;
else if(oc==6)return 2;
else if(oc==7)return 2;
else if(oc==8)return 2;
else if(oc==9)return 2;
else if(oc==10)return 2;
else if(oc==11)return 1;
else if(oc==12)return 1;
else if(oc==13)return 1;
else if(oc==14)return 2;
else if(oc==15)return 1;
else if(oc==16)return 0;
else if(oc==17)return 0;
else if(oc==18)return 0;
else if(oc==19)return 0;
else if(oc==20)return 0;
else return 0;}
//list of opcodes: [NOP, MOV, LOADI, LOADS, INC, DEC, ADD, SUB, MUL, DIV, SWP, POP, PUSH, PUSHI, SYSCALL, CALL, RET, INLINE_STR_START, INLINE_STR_END, INLINE_ID_REF, END, ]
typedef enum{
WRITEVAL,		//Writes a register value to screen	@WRITEVAL r0
WRITEL,		//Writes a line to screen	@WRITEL sr0
GETLINE,		//Gets a line from stdinput	@GETLINE sr0
} SYSCALLS;
//[[[end]]]
//terms: iv: immediate value

typedef enum {
	REG_RETVAL = 50, REG_ACCUMULATE, REG_RESULT
} SPECIAL_REGISTERS;

typedef enum {
	LBL_START = END, LBL_END
} KEYWORDS;