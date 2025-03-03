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

void LexicalAnalyzer::variablesCheck(std::vector<Term*>& st, std::map<std::string, double>& data) {
	for (int i = 0; i < st.size(); i++) {
		Variable* pv = dynamic_cast<Variable*>(st[i]);
		if (pv != nullptr) {
			std::string name = pv->getName();
			Function<double>* pf = nullptr;
			if (i != st.size() - 1) pf = dynamic_cast<Function<double>*>(st[i + 1]);
			if (i == st.size() - 1 || pf == nullptr || !pf->is_assign()) {
				if (data.count(name) == 1) {
					delete(st[i]);
					st[i] = new Number<>(data[name]);
				}
				else throw "you're trying to use undefine variable";
			}
		}
	}
}

std::vector<Term*> LexicalAnalyzer::analysis(const std::string& s, std::map<std::string, double>& data) {

	std::vector<Term*> st;

	for (int i = 0; i < s.size(); i++) {

		if (s[i] == ' ') continue;

		if (s[i] == '(') st.push_back(new Bracket(brackets::OPENING));
		else if (s[i] == ')') st.push_back(new Bracket(brackets::CLOSING));

		else if (s[i] >= '0' && s[i] <= '9') {
			
			double num = this->readNum(s, i);
			st.push_back(new Number<>(num));

			i--;
		}

		else if (s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z') {
			std::string name = this->readVar(s, i);

			st.push_back(new Variable(name));

			i--;
		}

		else if (s[i] == '+') st.push_back(new Function<double, double>(std::plus<double>(), 1));
		else if (s[i] == '-') {
			Variable* pv = nullptr;
			Number<>* pn = nullptr;
			if (!st.empty()) {
				pv = dynamic_cast<Variable*>(st[st.size()-1]);
				pn = dynamic_cast<Number<>*>(st[st.size() - 1]);
			}
			if (pv != nullptr || pn != nullptr) st.push_back(new Function<double, double>(std::minus<double>(), 1));
			else st.push_back(new Function<double>(std::negate<double>(), -1));
		} 
		else if (s[i] == '*') st.push_back(new Function<double, double>(std::multiplies<double>(), 0));
		else if (s[i] == '/') st.push_back(new Function<double, double>(std::divides<double>(), 0));
		else if (s[i] == '=')st.push_back(new Function<double>(nullptr, 2));

		else throw "your expression isn't correct";

	}

	this->variablesCheck(st, data);

	return st;
}