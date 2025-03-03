#pragma once
#include <functional>
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


template <class T, class... Args>
class Function : public Term {
private:

	std::function<double(T, Args...)> func;
	int pr = 0;

public:
	Function(const std::function<double(T,Args...)>& func, int pr) : Term(), func(func), pr(pr){}

	bool is_div() {
		if (this->func.target<std::divides<double>>()) return true;
		else return false;
	}

	bool is_assign() {
		if (this->func == nullptr) return true;
		else return false;
	}

	//T f(T a, Args... args);
	
	double f(double a) { return this->func(a); }

	double f(double a, double b) { 
		double ans = this->func(a, b);
		return ans;
	}

	int getPriority() { return this->pr; }
};


