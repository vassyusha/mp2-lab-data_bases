#pragma once
#include "polynoms.h"
#include "term.h"
#include <string>

class translator {
	Polynom P;
public:
	translator(const std::string& str){
		if (syntaxAnalyzer(lexicalAnalyzer(str))) {
			P = createPolynom(lexicalAnalyzer(str));
		}
		else throw "Incorrect expresion\n";
	}
	Polynom getPolynom() {
		return P;
	}
	std::pair<double,int> readD(const std::string& str,int i) {
		int j = i + 1;
		std::string num = "";
		num += str[i];
		while (j < str.size() && ('0' <= str[j] && str[j] <= '9')) {
			num += str[j];
			j++;
		}
		if (str[j] == '.') {
			num += str[j];
			j++;
			while (j < str.size() && ('0' <= str[j] && str[j] <= '9')) {
				num += str[j];
				j++;
			}
		}
		else num += ".0";
		return {stod(num), j };
	}
	std::pair<int, int> readI(const std::string& str, int i) {
		int j = i + 1;
		std::string num = "";
		while (j < str.size() && ('0' <= str[j] && str[j] <= '9')) {
			num += str[j];
			j++;
		}
		if (num == "") throw "Incorrect power of variable\n";
		else return {stoi(num),j};
	}
	std::vector<Term*> lexicalAnalyzer(const std::string& str) {
		std::vector<Term*> v;
		int i = 0;
		while (i < str.size()) {
			if ((str[i] == '+') || (str[i] == '-')) {
				v.push_back(new Sign(str[i]));
				i++;
				continue;
			}
			if (str[i] == '^') {
				v.push_back(new Power(readI(str,i).first));
				i= readI(str, i).second;
				continue;
			}
			if ('0' <= str[i] && str[i] <= '9') {
				v.push_back(new Coef(readD(str, i).first)); //коэффицент пока что без учёта знака 
				i = readD(str, i).second;
				continue;
			}
			if (str[i] == 'x' || str[i] == 'y' || str[i] == 'z') {
				v.push_back(new Variable(str[i]));
				i++;
				continue;
			}
			else throw "Incorrect symbol\n";
		}
		v.push_back(new End());
		return v;
	}
	bool syntaxAnalyzer(const std::vector<Term*>& terms) {
		int i = 0;
		int flag = 0;
		int count = 0;
		while (i < terms.size()) {
			switch (flag) {
			case 0:
				if (dynamic_cast<Sign*>(terms[i])) {
					flag = 1;
					break;
				}
				else {
					flag = 3;
					break;
				}
			case 1:
				if (dynamic_cast<Coef*>(terms[i])) {
					flag = 2;
					break;
				}
				else {
					flag = 3;
					break;
				}
			case 2:
				if (dynamic_cast<Variable*>(terms[i]) && dynamic_cast<Variable*>(terms[i])->getName()=='x') {
					flag = 4;
					count++;
					break;
				}
				if (dynamic_cast<End*>(terms[i])) {
					flag = 6;
					break;
				}
				else {
					flag = 3;
					break;
				}
			case 4:
				if (dynamic_cast<Power*>(terms[i])) {
					flag = 5;
					break;
				}
				else {
					flag = 3;
					break;
				}
			case 5:
				if (dynamic_cast<Variable*>(terms[i]) && count == 1) {
					flag = 4;
					count++;
					break;
				}
				if (dynamic_cast<Variable*>(terms[i]) && count == 2) {
					flag = 4;
					count++;
					break;
				}
				if (dynamic_cast<End*>(terms[i])&&count==3) {
					flag = 6;
					break;
				}
				if (dynamic_cast<Sign*>(terms[i])&& count == 3) {
					flag =1;
					count = 0;
					break;
				}
				else {
					flag = 3;
					break;
				}
			case 6: break;
			}
			if (flag == 3) return 0;
			i++;
		}
		return 1;
	}
	Polynom createPolynom(const std::vector<Term*>& terms) {
		int i = 0;
		int flag = 0;
		Polynom P;
		while (!dynamic_cast<End*>(terms[i])) {
			Monom m;
			if (dynamic_cast<Sign*>(terms[i])->getFlag()==0) {
				i++;
				if (terms.size() == 3) {
					Coef c(-dynamic_cast<Coef*>(terms[i])->getData());
					m = Monom(c.getData(), {0,0,0});
					i++;
				}
				else {
					Coef c(-dynamic_cast<Coef*>(terms[i])->getData()); // i на коэфиценте 
					std::vector<int> powers = { dynamic_cast<Power*>(terms[i + 2])->getData(),dynamic_cast<Power*>(terms[i + 4])->getData(),dynamic_cast<Power*>(terms[i + 6])->getData() };
					m = Monom(c.getData(), powers);
					i = i + 7;
				}
			}
			else {
				i++;
				if (terms.size() == 3) {
					Coef c(dynamic_cast<Coef*>(terms[i])->getData());
					m = Monom(c.getData(), { 0,0,0 });
					i++;
				}
				else {
					Coef c(dynamic_cast<Coef*>(terms[i])->getData()); // i на коэфиценте 
					std::vector<int> powers = { dynamic_cast<Power*>(terms[i + 2])->getData(),dynamic_cast<Power*>(terms[i + 4])->getData(),dynamic_cast<Power*>(terms[i + 6])->getData() };
					m = Monom(c.getData(), powers);
					i = i + 7;
				}
			}
			P += m;
		}
		return P;
	}
	
};

