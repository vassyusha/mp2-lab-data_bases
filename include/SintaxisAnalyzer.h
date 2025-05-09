#pragma once
/// здесь будет синтаксический анализ, т.е. проверка на скобки, затем проверка на корректность выражения
#include <stack>
#include <string>
#include <vector>
#include "Term.h"

class SintaxisAnalyzer
{
public:
	bool bracketCorrectness(const std::vector<Term*>&);

	bool analysis(const std::vector<Term*>&);

	std::vector<Term*> poland(std::vector<Term*>&);
};

