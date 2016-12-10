#pragma once
#include "assembler.h"
#include <iostream>
#include <fstream>
#include <sstream>

class parser
{
public:
	parser();
	~parser();
	BASE_TYPE *parse_file(const char* fname, int &sz) {
		stringstream streem;
		ifstream fso(fname);
		streem << fso.rdbuf();
		//printf(streem.str().c_str());
		return parse(streem.str().c_str());
	}

	template<class TContainer>
	bool begins_with(const TContainer& input, const TContainer& match)
	{
		return input.size() >= match.size()
			&& equal(match.begin(), match.end(), input.begin());
	}
	BASE_TYPE *parse(const char* texts) {
		istringstream ins;
		ins.str(texts);
		for (string line; getline(ins, line); ) {
			if (line[0] == ';' || line.size() <= 1)continue;
			int i = 0;
			while (i < line.size() && (line[i] == ' ' || line[i] == '\t' || line[i] == '\0' || line[i] == '\r' || line[i] == '\n')) {
				i++;
			}
			line = line.substr(i);
			if (begins_with<string>(line, "LOADI")) {
				printf("%i %c[%s]\n", i, line[i], line.c_str());
			}
			else if (begins_with<string>(line, "LOADS")) {

			}
			//printf("%i %c[%s] %i\n", i, line[i], line.c_str(), _strcompare(line, string("LOAD"), i));
		skip:
			continue;
		}
		return nullptr;
	}
};

