#pragma once
#include "assembler.h"
#include <iostream>
#include <fstream>
#include <sstream>

class pmacro {
private:
	string m_name;
public:
	string name() const { return m_name; }

};
#include <map>
class parser
{
public:
	parser();
	~parser();
	static map<BASE_TYPE, string> m_lblmap;
	static void pushlbl(BASE_TYPE hsh, string na);
	static string getlblname(BASE_TYPE hsh);
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

