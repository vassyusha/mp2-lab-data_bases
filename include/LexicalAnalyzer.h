#pragma once
#include <stack>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <map>

#include "Term.h"
#include "translatorP.h"
/// <summary>
/// класс, отвечающий за лексический предложения 
/// здесь будет парсер выражения на массив термов
/// есть вайб добавить еще отдельный метод, отвечающий за считывание числа, но это уже при расширении под даблы
/// </summary>
class LexicalAnalyzer
{
private:
	double readNum(const std::string& s, int& pos);
	std::string readVar(const std::string& s, int& pos);
	void variablesCheck(std::vector<Term*>& st, std::map<std::string, Polynom>& data);
public:
	
	std::vector<Term*> analysis(const std::string& s, std::map<std::string, Polynom>&);
};

