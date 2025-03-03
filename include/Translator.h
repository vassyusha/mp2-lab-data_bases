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
class Translator
{
private:
	void destr(std::vector<Term*>&);
public:
	double translate(const std::string&, std::map<std::string, double>&);

	
};

