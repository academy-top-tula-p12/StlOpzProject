#pragma once
#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <algorithm>

using namespace std;

class MathExpression
{
	string expression;
	string expressionOpz;
	map<string, double> variables;

	const string bracketsOpen{ "([{" };
	const string bracketsClose{ ")]}" };

	const string operatorsAdditive{ "+-" };
	const string operatorsMultiplex{ "*/" };

public:
	MathExpression() : expression{ "" } {}
	MathExpression(string expression)
		: expression{ expression } {}
	MathExpression(const char* expression)
		: expression{ string(expression) } {}

	string& Expression();
	string& ExpressionOpz();

	double& operator[](const char* varName);
	double& operator[](string varName);

	int CheckBrackets();
	void OpzCreate();
	double OpzCalculate();
	double Calculate();
};

