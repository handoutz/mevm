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
		return parse(streem.str().c_str(), sz);
	}

	template<class TContainer>
	bool begins_with(const TContainer& input, const TContainer& match)
	{
		return input.size() >= match.size()
			&& equal(match.begin(), match.end(), input.begin());
	}
	BASE_TYPE *parse(const char* texts, int &sz);
};

