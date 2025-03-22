#include "Translator.h"

void Translator::destr(std::vector<Term*>& v) {
	for (int i = 0; i < v.size(); i++) delete(v[i]);
}


Polynom Translator::translate(const std::string& s, std::map<std::string, Polynom>& data) {

	LexicalAnalyzer lan;
	std::vector<Term*> v = lan.analysis(s, data);

	SintaxisAnalyzer san;
	if (!san.bracketCorrectness(v)) {
		this->destr(v);
		throw "smth wrong with brackets";
	}
	if (!san.analysis(v)) {
		this->destr(v);
		throw "smth wrong with sintaxis";
	}
	v = san.poland(v);

	Solver sl;

	Polynom result = sl.solve(v, data);
	this->destr(v);
	return result;
	
}