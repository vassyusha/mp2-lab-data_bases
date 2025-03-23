#include "Solver.h"
#include <iostream>
#include <map>

Polynom Solver::solve(const std::vector<Term*>& data, std::map<std::string, Polynom>& database) {
	std::stack<Polynom> values;

	Number<>* pn;
	Polynomial* pp;
	Function<Polynom>* pf1;
	Function<Polynom, Polynom>* pf2;

	for (int i = 0; i < data.size(); i++) {
		pp = dynamic_cast<Polynomial*>(data[i]);
		{
			if (pp != nullptr) values.push(pp->getPolynom());
		}

		pf1 = dynamic_cast<Function<Polynom>*>(data[i]);
		pf2 = dynamic_cast<Function<Polynom, Polynom>*>(data[i]);
		if(pf1 != nullptr || pf2 != nullptr){


			Polynom res;

			if (pf2 != nullptr) {
				Polynom second = values.top();
				values.pop();
				Polynom first = values.top();
				values.pop();
	
				res = pf2->f(first, second);
			}

			if (pf1 != nullptr) {
				if (pf1->is_assign()) {
					Polynom value = values.top();
					Variable* pv = dynamic_cast<Variable*>(data[0]);//да, это костыль
					database[pv->getName()] = value;
					return value;
				}
				Polynom value = values.top();
				values.pop();

				res = pf1->f(value);
			}
			
			values.push(res);
		}
	}

	return values.top();
}