#pragma once
#include <exception>
#include <string>
#include <sstream>
using namespace std;
class VMException : exception
{
public:
	explicit VMException(const char* msg):m_msg(msg){}
	explicit VMException(const string& msg):m_msg(msg){}
	virtual ~VMException() throw(){}
	
	virtual string name()=0;
	virtual const char* what() {
		stringstream b;
		b << name() << ": " << str();
		return b.str().c_str();
	}
	virtual string str() const{
		return m_msg;
	}
protected:
	string m_msg;
};
class NullRefException : public VMException{
public:
	explicit NullRefException(string msg):VMException(msg){}
	virtual string name(){return "NullReference";}
};
