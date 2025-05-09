#pragma once
#include <map>
#include <string>
#include <iostream>
#include "Translator.h"
template<class database>
class SolutionManager
{
private:
	database data;

	std::string inform = "";

public:
	void manage() {

		std::cout << "Numbers:\nAll numbers used in calculations are real numbers.\n3 number input formats are supported :\n- Without dot : 123\n- With a dot, but without a fractional part : 12.\n- Just with a dot : 12.46\n\n";
		std::cout << "Variables:\nThe variable name can include the characters 'A - Z', 'a - z', '0 - 9', but the variable name cannot begin with a digit.\nExamples of acceptable names : aa, a1, myVar\nBefore using a variable, it must be initialized.The value of a variable can be changed.\nAssign a value to a variable in the format : <variable name> = <new value(possibly calculated)>.\n\n";
		std::cout << "Functions:\nAcceptable operations :\n- Addition : 1 + 2\n\- Subtraction : 3.5 - 2\n- Multiplication : 4 * 7.3\n- Division : 3.7 / 8.2\n- Unary minus sign : -a1\n\n";
		std::cout << "Parentheses:\nYou can also use parentheses in calculations.\nThe parenthesis sequence must be correct.\n\n";
		std::cout << "Now you can input your expression\n";

		std::string s;
		Translator<database> tr;

		while (true) {
			std::getline(std::cin, s);
			try {
				//tr.translate(s, this->data).print();
				//std::cout << "\n\n";
				std::cout << tr.translate(s, this->data) << "\n\n";
			}
			catch (const char* s) { std::cout << s << "\n\n"; }
		}
	}
};

