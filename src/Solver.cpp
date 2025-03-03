#include "Solver.h"
#include <iostream>
#include <map>

double Solver::solve(const std::vector<Term*>& data, std::map<std::string, double>& database) {
	std::stack<double> values;

	Number<>* pn;
	Function<double>* pf1;
	Function<double, double>* pf2;

	for (int i = 0; i < data.size(); i++) {
		pn = dynamic_cast<Number<>*>(data[i]);
		if (pn != nullptr) values.push(pn->getValue());

		pf1 = dynamic_cast<Function<double>*>(data[i]);
		pf2 = dynamic_cast<Function<double, double>*>(data[i]);
		if(pf1 != nullptr || pf2 != nullptr){


			double res;

			if (pf2 != nullptr) {
				double second = values.top();
				values.pop();
				double first = values.top();
				values.pop();
				
				if (pf2->is_div()) if (second == 0) throw "you can't divide by zero";
	
				res = pf2->f(first, second);
			}

			if (pf1 != nullptr) {
				if (pf1->is_assign()) {
					double value = values.top();
					Variable* pv = dynamic_cast<Variable*>(data[0]);//да, это костыль
					database[pv->getName()] = value;
					return value;
				}
				double value = values.top();
				values.pop();

				res = pf1->f(value);
			}
			
			values.push(res);
		}
	}

	return values.top();
}