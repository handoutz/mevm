#pragma once
#include <string>
#include <functional>
#include <vector>
using namespace std;

namespace str {
	vector<string> tokenize(char* str, char* delim);
	vector<string> tokenize(const char* str, char* delim);
	size_t indexof(string haystack, string needle, size_t start=0);
	string padtolen(string s, int len);
}
namespace linq {

	template<typename T>
	class IEnumerator {
	public:
		virtual bool next() = 0;
		virtual void reset() = 0;
		virtual T current() = 0;
	};
	template<typename T>
	class ArrayEnumerator : public IEnumerator<T> {
	private:
		T* m_src;
		size_t m_size;
		size_t m_pos;
	public:
		ArrayEnumerator(T* src, size_t sz) :m_src(src), m_size(sz) {}

		virtual bool next() override;
		virtual void reset() override;
		virtual T current() override;
	};

}