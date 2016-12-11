#pragma once
#include <stdint.h>
#include "StringAlgos.h"
inline uint32_t pack_helper(uint32_t c0, uint32_t c1, uint32_t c2, uint32_t c3) {
	return c0 | (c1 << 8) | (c2 << 16) | (c3 << 24);
}

inline uint32_t pack(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3) {
	return pack_helper(c0, c1, c2, c3);
}

template <int N>
inline uint8_t unpack_u(uint32_t packed) {
	// cast to avoid potential warnings for implicit narrowing conversion
	return static_cast<uint8_t>(packed >> (N * 8));
}

template <int N>
inline int8_t unpack_s(uint32_t packed) {
	uint8_t r = unpack_u<N>(packed);
	return (r <= 127 ? r : r - 256); // thanks to caf
}
template<typename T>
inline char* unpack_str4(T val) {
	char *ss = (char*)calloc(5, sizeof(char));
	ss[0] = unpack_s<3>(val);
	ss[1] = unpack_s<2>(val);
	ss[2] = unpack_s<1>(val);
	ss[3] = unpack_s<0>(val);
	ss[4] = '\0';
	return ss;
}

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
inline std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}
#include "parser.h"
template<typename T>
inline void dodump(std::vector<T> &curline, int &skipOpcodeNames, BASE_TYPE& lastopcode) {
	for (size_t j = 0; j < curline.size(); j++) {
		BASE_TYPE currentBt = curline.at(j);
		if (skipOpcodeNames > 0)skipOpcodeNames--;
		if (curline.at(j) < OPCODES::END && skipOpcodeNames == 0) {
			lastopcode = curline.at(j);
			auto autoname = get_opcode_name((OPCODES)lastopcode);
			skipOpcodeNames = get_n_params((OPCODES)lastopcode) + 1;
			printf("%s  ", str::padtolen(autoname, 8).c_str());
		}
		else if (lastopcode == CALL || (lastopcode >= JE || lastopcode <= JLT)) {
			auto lblname = parser::getlblname(currentBt);
			printf("%s  ", str::padtolen(lblname, 8).c_str());
		}
		else {
			char* ss = unpack_str4(currentBt);
#define rplchr(num) do{ char ccc=ss[num]; if(ccc!='\0'&&(ccc<33||ccc>122))ss[num]='#'; }while(0);
			rplchr(0);
			rplchr(1);
			rplchr(2);
			rplchr(3);
			printf("%c %c %c %c  ", ss[0], ss[1], ss[2], ss[3]);
		}
	}
	curline.clear();
	std::cout << std::endl;
}
template<typename T>
inline void hexDump(char* desc, T *addr, int len, int linelen = 6, int start=0) {
	std::cout << desc << std::endl;
	std::vector<T> curline;
	BASE_TYPE lastopc = -1;
	int skippers = 0, i = start;
	printf("%02i: ",i);
	for (i = start; i < len; i++) {
		T c = addr[i];
		printf("%08x ", c);
		curline.push_back(c);
		if (i % linelen == 0 && i > 0) {
			printf("\n%02i: ", i - linelen);
			dodump(curline, skippers, lastopc);
			printf("%02i: ", i);
		}
	}
	if (curline.size() > 0) {
		printf("\n%02i: ", i - linelen);
		dodump(curline, skippers, lastopc);
	}
	std::cout << std::endl << std::endl;
}
inline void _hexDump(char *desc, void *addr, int len) {
	int i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;

	// Output description if given.
	if (desc != NULL)
		printf("%s:\n", desc);

	if (len == 0) {
		printf("  ZERO LENGTH\n");
		return;
	}
	if (len < 0) {
		printf("  NEGATIVE LENGTH: %i\n", len);
		return;
	}

	// Process every byte in the data.
	for (i = 0; i < len; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
				printf("  %s\n", buff);

			// Output the offset.
			printf("  %04x ", i);
		}

		// Now the hex code for the specific character.
		printf(" %02x", pc[i]);

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buff[i % 16] = '.';
		else
			buff[i % 16] = pc[i];
		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		printf("   ");
		i++;
	}

	// And print the final ASCII bit.
	printf("  %s\n", buff);
}

inline unsigned long _hash_sdbm(unsigned char *str)
{
	unsigned long hash = 0;
	int c;

	while (c = *str++)
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}