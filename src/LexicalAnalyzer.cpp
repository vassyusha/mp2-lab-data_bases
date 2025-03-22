#include "LexicalAnalyzer.h"

double LexicalAnalyzer::readNum(const std::string& s, int& pos) {

	double res = 0;
	double temp = 0.1;

	int state = 0;

	for (pos; pos < s.size(); pos++) {
		switch (state) {
		case 0:
			if (s[pos] == '.') state = 1;
			else {
				if (s[pos] >= '0' && s[pos] <= '9') (res *= 10) += (s[pos] - '0');
				else return res;
			}
			break;
		case 1:
			if (s[pos] >= '0' && s[pos] <= '9') {
				res += (s[pos] - '0') * temp;
				temp /= 10;
			}
			else return res;
		}
	}
	return res;
}

std::string LexicalAnalyzer::readVar(const std::string& s, int& pos) {
	std::string name;
	while (pos < s.size() && (s[pos] >= 'a' && s[pos] <= 'z' || s[pos] >= 'A' && s[pos] <= 'Z' || s[pos] >= '0' && s[pos] <= '9')) name += s[pos++];

	return name;
}

void LexicalAnalyzer::variablesCheck(std::vector<Term*>& st, std::map<std::string, Polynom>& data) {
	for (int i = 0; i < st.size(); i++) {
		Variable* pv = dynamic_cast<Variable*>(st[i]);
		if (pv != nullptr) {
			std::string name = pv->getName();
			Function<Polynom>* pf = nullptr;
			if (i != st.size() - 1) pf = dynamic_cast<Function<Polynom>*>(st[i + 1]);
			if (i == st.size() - 1 || pf == nullptr || !pf->is_assign()) {
				if (data.count(name) == 1) {
					delete(st[i]);
					st[i] = new Polynomial(data[name]);
				}
				else throw "you're trying to use undefine variable";
			}
		}
	}
}

std::vector<Term*> LexicalAnalyzer::analysis(const std::string& s, std::map<std::string, Polynom>& data) {

	std::vector<Term*> st;

	for (int i = 0; i < s.size(); i++) {

		if (s[i] == ' ') continue;

		if (s[i] == '(') st.push_back(new Bracket(brackets::OPENING));
		else if (s[i] == ')') st.push_back(new Bracket(brackets::CLOSING));

		else if (s[i] >= '0' && s[i] <= '9') {
			
			double num = this->readNum(s, i);
			st.push_back(new Polynomial(Polynom(std::vector<Monom>(1, Monom(num)))));

			i--;
		}

		else if (s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z') {
			std::string name = this->readVar(s, i);

			st.push_back(new Variable(name));

			i--;
		}

		else if (s[i] == '+') st.push_back(new Function<Polynom, Polynom>(std::plus<Polynom>(), 1));
		else if (s[i] == '-') {
			Variable* pv = nullptr;
			Number<>* pn = nullptr;
			if (!st.empty()) {
				pv = dynamic_cast<Variable*>(st[st.size()-1]);
				pn = dynamic_cast<Number<>*>(st[st.size() - 1]);
			}
			if (pv != nullptr || pn != nullptr) st.push_back(new Function<Polynom, Polynom>(std::minus<Polynom>(), 1));
			else st.push_back(new Function<Polynom>(std::negate<Polynom>(), -1));
		} 
		else if (s[i] == '*') st.push_back(new Function<Polynom, Polynom>(std::multiplies<Polynom>(), 0));
		else if (s[i] == '=') {
			st.push_back(new Function<Polynom>(nullptr, 2));
			i++;
			while (s[i] == ' ') i++;
			int contr = i;
			translator t;
			Polynomial* pp;
			try {
				pp = new Polynomial(t.translate(s, i));
				st.push_back(pp);
			}catch (const char* s) {
				i = contr - 1;
			}
			
		}

		else throw "your expression isn't correct";

	}

	this->variablesCheck(st, data);

	return st;
}