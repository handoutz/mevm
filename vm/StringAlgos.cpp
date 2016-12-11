#include "stdafx.h"
#include <vector>
#include "StringAlgos.h"
using namespace std;
std::vector<std::string> str::tokenize(char* str, char* delim)
{
	char* p;
	vector<string> res;
	p = strtok(str, delim);
	while (p != NULL) {
		res.push_back(string(p));
		p = strtok(nullptr, delim);
	}
	return res;
}


size_t str::indexof(string haystack, string needle, size_t start)
{
	auto res = haystack.find(needle, start);
	return res;
}
#include <sstream>
std::string str::padtolen(string s, int len)
{
	int n = s.length();
	stringstream ss;
	if (n >= len)
		ss << s.substr(0, len);
	else {
		ss << s;
		while (n++ < len-1)
			ss << ' ';
	}
	return ss.str();
}

std::vector<std::string> str::tokenize(const char* str, char* delim)
{
	return tokenize((char*)str, delim);
}

template<typename T>
bool linq::ArrayEnumerator<T>::next()
{
	return ++m_pos >= m_size;
}
template<typename T>
T linq::ArrayEnumerator<T>::current()
{
	return m_src[m_pos];
}

template<typename T>
void linq::ArrayEnumerator<T>::reset()
{
	m_pos = 0;
}