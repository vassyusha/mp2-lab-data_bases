#pragma once
#include <string>
#include <map>
#include "LexicalAnalyzer.h"
#include "SintaxisAnalyzer.h"
#include "Solver.h"
/// <summary>
/// управляющий класс, который на вход получает строку - выражение, значение которого необходимо вычислить
/// возвращает - искомое значение (ну или ничего не возвращает, если вычислить не просят)
/// </summary>
template<class database>
class Translator
{
private:
	void destr(std::vector<Term*>& v) {
		for (int i = 0; i < v.size(); i++) delete(v[i]);
	}
public:
	Polynom translate(const std::string& s, database& data) {
		LexicalAnalyzer<database> lan;
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

		Solver<database> sl;

		Polynom result = sl.solve(v, data);
		this->destr(v);
		return result;
	}

};

