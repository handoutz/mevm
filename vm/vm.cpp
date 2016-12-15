// vm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "virtuo.h"
#include "VMException.h"
#include "_tests.h"
#include "parser.h"
#include <iostream>
#include "ConsoleUi.h"
using namespace std;

inline TestProgram** get_test_progs(int &n) {
	n = 3;
	auto p = new TestProgram*[n];
	p[0] = test_prog_assembleme();
	p[1] = test_prog_conditionals();
	p[2] = test_prog_hello();
	return p;
}

int _tmain(int argc, _TCHAR* argv[])
{
    while (1) {
        try {
            TestProgram** progs = nullptr;
            int nProgs = 0;
            progs = get_test_progs(nProgs);
            cout << "Choose program" << endl;
            for (int i = 0; i < nProgs; i++) {
                cout << i + 1 << ". " << progs[i]->name << endl;
            }
            cout << "Selection: ";
            int iSel = 0;
            cin >> iSel;
            iSel--;;
            TestProgram* program = progs[iSel];
            cout << "Running program #" << iSel << ": " << program->name << endl;
            virtuo v;
            v.run(program->program, program->sz);
            v.dmpState();
        }
        catch (std::exception &e) {
            printf("Error!: %s\n", e.what());
        }
        catch (...) {
            printf("Error!: unknown");
        }
    }
	return 0;
}

