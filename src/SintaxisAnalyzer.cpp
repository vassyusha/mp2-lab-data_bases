#include "SintaxisAnalyzer.h"

bool SintaxisAnalyzer::bracketCorrectness(const std::vector<Term*>& data) {
	std::stack<Bracket*> st;
	Bracket* pb;
	for (int i = 0; i <data.size(); i++) {
		pb = dynamic_cast<Bracket*>(data[i]);
		if (pb != nullptr) {
			if (pb->isOpening()) st.push(pb);
			if (pb->isClosing()) {
				if (st.size() == 0) return false;
				if (st.top()->isOpening()) st.pop();
				else return false;
			}
		}
	}
	if (st.size() != 0) return false;
	return true;
}

bool SintaxisAnalyzer::analysis(const std::vector<Term*>& data) {
	int state = -2;

	Number<>* pn;
	Function<Polynom>* pf1;
	Function<Polynom, Polynom>* pf2;
	Bracket* pb;
	Variable* pv;
	Polynomial* pp;

	for (int i = 0; i < data.size(); i++) {
		switch (state) {
		case -2://ожидаем либо переменную, либо выражение
			pv = dynamic_cast<Variable*>(data[i]);
			if (pv != nullptr) state = -1;
			else {
				state = 0;
				i--;
			}
			break;
		case -1://получили переменную и ожидаем после нее равно
			pf1 = dynamic_cast<Function<Polynom>*>(data[i]);
			if (pf1 != nullptr) {
				if (pf1->is_assign()) state = 5;
				else return false;
			}
			else return false;
			break;
		case 0://начало выражения, ожидаем обязательно число или унарный минус
			pp = dynamic_cast<Polynomial*>(data[i]);
			if (pp != nullptr) state = 1;
			else {
				pb = dynamic_cast<Bracket*>(data[i]);
				if (pb != nullptr && pb->isOpening()) state = 2;
				else {
					pf1 = dynamic_cast<Function<Polynom>*>(data[i]);
					if (pf1 != nullptr && !(pf1->is_assign())) state = 4;
					else return false;
				} 
			}
			break;
		case 1:
			pf2 = dynamic_cast<Function<Polynom, Polynom>*>(data[i]);
			if (pf2 != nullptr) state = 0;
			else {
				pb = dynamic_cast<Bracket*>(data[i]);
				if (pb != nullptr && pb->isClosing()) state = 3;
				else return false;
			}
			break;
		case 2:
			pn = dynamic_cast<Number<>*>(data[i]);
			if (pn != nullptr) state = 1;
			else {
				pb = dynamic_cast<Bracket*>(data[i]);
				pf1 = dynamic_cast<Function<Polynom>*>(data[i]);
				if (pb != nullptr && pb->isOpening()) state = 2;
				else if (pf1 != nullptr && !(pf1->is_assign())) state = 4;
				else return false;
			}
			break;
		case 3:
			pf2 = dynamic_cast<Function<Polynom, Polynom>*>(data[i]);
			if (pf2 != nullptr) state = 0;
			else {
				pb = dynamic_cast<Bracket*>(data[i]);
				if (pb != nullptr && pb->isClosing()) state = 3;
				else return false;
			}
			break;
		case 4://получили унарный минус, теперь нормис получить число или скобку
			pn = dynamic_cast<Number<>*>(data[i]);
			pb = dynamic_cast<Bracket*>(data[i]);
			if (pb != nullptr && pb->isOpening()) state = 2;
			else if (pn != nullptr) state = 1;
			else return false;
			break;
		case 5:
			pp = dynamic_cast<Polynomial*>(data[i]);
			if (pp != nullptr) return true;
			return false;
			break;
		}
	}
	if (state == 1 || state == 3) return true;
	return false;
}

std::vector<Term*> SintaxisAnalyzer::poland(std::vector<Term*>& data) {
	Number<> *pn;
	Polynomial* pp;
	Function<Polynom>* pf1,*pfc1;
	Function<Polynom, Polynom>* pf2,*pfc2;
	Bracket* pb;
	Variable* pv;

	std::vector<Term*> ne;
	std::stack<Term*> st;
	for (int i = 0; i < data.size(); i++) {
		pp = dynamic_cast<Polynomial*>(data[i]);
		if (pp != nullptr) ne.push_back(data[i]);

		pv = dynamic_cast<Variable*>(data[i]);
		if (pv != nullptr) ne.push_back(data[i]);

		pb = dynamic_cast<Bracket*>(data[i]);
		if (pb != nullptr) {
			if (pb->isOpening()) st.push(data[i]);
			else {
				pb = dynamic_cast<Bracket*>(st.top());
				while (pb == nullptr) {
					ne.push_back(st.top());
					st.pop();
					pb = dynamic_cast<Bracket*>(st.top());
				}
				delete(st.top());
				st.pop();
			}
		}

		pfc1 = dynamic_cast<Function<Polynom>*>(data[i]);
		pfc2 = dynamic_cast<Function<Polynom, Polynom>*>(data[i]);

		if (pfc1 != nullptr || pfc2 != nullptr) {
			if(!st.empty()) pb = dynamic_cast<Bracket*>(st.top());
			if (st.empty() || pb != nullptr) st.push(data[i]);
			else {
				pf1 = dynamic_cast<Function<Polynom>*>(st.top());
				pf2 = dynamic_cast<Function<Polynom, Polynom>*>(st.top());
				int cpr;
				if (pfc1 != nullptr)cpr = pfc1->getPriority();
				if (pfc2 != nullptr)cpr = pfc2->getPriority();
				while (pf1!= nullptr && cpr >= pf1->getPriority() || pf2 != nullptr && cpr >= pf2->getPriority()) {
					ne.push_back(st.top());
					st.pop();
					if (st.empty()) break;
					pf1 = dynamic_cast<Function<Polynom>*>(st.top());
					pf2 = dynamic_cast<Function<Polynom, Polynom>*>(st.top());
				}
				st.push(data[i]);
			}
		}
	}

	while (!st.empty()) {
		ne.push_back(st.top());
		st.pop();
	}

	return ne;
}