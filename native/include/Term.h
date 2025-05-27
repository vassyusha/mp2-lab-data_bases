#pragma once
#include <functional>
#include "polynoms.h"
/// <summary>
/// полиморфный класс, который будет отвечать за сущности, участвующие в вычислениях
/// </summary>
enum brackets {
	OPENING,
	CLOSING
};
 
class Term{

public:
	virtual ~Term(){}

};

class Bracket : public Term {
private:
	int state;

public:
	Bracket(int state):Term(), state(state){}

	bool isOpening() { return this->state == brackets::OPENING; }
	bool isClosing() { return this->state == brackets::CLOSING; }

};

class Variable : public Term {
private:
	std::string name;
public:
	Variable(std::string name) :Term(), name(name){}
	std::string getName() { return this->name; }
};

template <class T = double>
class Number : public Term {
private:
	T value;
public:
	Number(const T& value) :Term(), value(value){}

	T& getValue() { return this->value; }
};


template <class T = Polynom, class... Args>
class Function : public Term {
private:

	std::function<T(T, Args...)> func;
	int pr = 0;

public:
	Function(const std::function<T(T,Args...)>& func, int pr) : Term(), func(func), pr(pr){}

	bool is_assign() {
		if (this->func == nullptr) return true;
		else return false;
	}

	//T f(T a, Args... args);
	
	T f(const T& a) const{ return this->func(a); }

	T f(const T& a, const T& b) const{ 
		T ans = this->func(a, b);
		return ans;
	}

	int getPriority() { return this->pr; }
};

class Polynomial : public Term {
private:
	Polynom p;

public:
	Polynomial(const Polynom& p): p(p){}

	Polynom getPolynom() const { return this->p; }
	Polynom& getPolynom() { return this->p; }
};


