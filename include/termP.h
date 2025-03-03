#pragma once

class Term {
public:
	virtual ~Term() {}
};
class End : public Term { };
class Variable : public Term {
	char name;
	double data = 3;
public:
	Variable(const char & s) : name(s) { }
	void setData(double& data_) {
		data = data_;
	}
	double getData() {
		return data;
	}
	char getName() {
		return name;
	}
};
class Coef : public Term {
	double data;
public:
	Coef(const double& data_) : data(data_) {};
	double getData() { return data;}
	void setData(double& data_) {data = data_;}
};
class Power : public Term {
	int data;
public:
	Power(const int& data_) : data(data_) {};
	int getData() { return data; }
};
class Sign : public Term {
	bool flag; // 1 - +, 0 - - 
public:
	Sign(const char& s) {
		if (s == '+') flag = 1;
		else flag = 0;
	}
	bool getFlag() { return flag; }
};
