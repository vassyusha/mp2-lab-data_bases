#pragma once
#include <stack>
#include <vector>
#include <map>
#include <string>
#include "Term.h"
/// <summary>
/// класс, который будет отвечать за непосредственное вычисление арифметических выражений
/// </summary>
class Solver
{
public:
	double solve(const std::vector<Term*>&, std::map<std::string, double>&);
};

