#pragma once

class Terma {
public:
	virtual ~Terma() {}
};
class End : public Terma { };
class Var : public Terma {
	char name;
	double data = 3;
public:
	Var(const char & s) : name(s) { }
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
class Coef : public Terma {
	double data;
public:
	Coef(const double& data_) : data(data_) {};
	double getData() { return data;}
	void setData(double& data_) {data = data_;}
};
class Power : public Terma {
	int data;
public:
	Power(const int& data_) : data(data_) {};
	int getData() { return data; }
};
class Sign : public Terma {
	bool flag; // 1 - +, 0 - - 
public:
	Sign(const char& s) {
		if (s == '+') flag = 1;
		else flag = 0;
	}
	bool getFlag() { return flag; }
};
