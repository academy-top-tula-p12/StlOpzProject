#include "MathExpression.h"

string& MathExpression::Expression()
{
    return expression;
}

string& MathExpression::ExpressionOpz()
{
    return expressionOpz;
}

double& MathExpression::operator[](string varName)
{
    if (!variables.contains(varName))
        variables.emplace(varName, 0.0);
    
    return variables[varName];
}

double& MathExpression::operator[](const char* varName)
{
    if (!variables.contains(varName))
        variables.emplace(varName, 0.0);

    return variables[varName];
}

int MathExpression::CheckBrackets()
{    
    const string brackets{ bracketsOpen + bracketsClose };
    
    stack<char> bracketsStack;
    
    for (int position{}; position < expression.length(); position++)
    {
        // not brackets
        if (brackets.find(expression[position]) == string::npos)
            continue;

        // opening brackets
        if (bracketsOpen.find(expression[position]) != string::npos)
        {
            bracketsStack.push(expression[position]);
            continue;
        }

        // closing brackets
        if (bracketsStack.empty())
            return position;

        int typeClose = bracketsClose.find(expression[position]);
        int typeOpen = bracketsOpen.find(bracketsStack.top());

        if (typeClose != typeOpen)
            return position;

        bracketsStack.pop();
    }

    return ((bracketsStack.empty()) ? -1 : expression.length());
}

void MathExpression::OpzCreate()
{
    stack<char> operatorsStack;

    for (int position{}; position < expression.length(); position++)
    {
        char symbol = expression[position];

        // space symbols
        if (symbol == ' ' || symbol == '\t')
            continue;

        // numbers
        if (isdigit(symbol) || symbol == '.')
        {
            string number{ "" };
            while (isdigit(symbol) ||
                symbol == '.' ||
                tolower(symbol) == 'e' &&
                position < expression.length())
            {
                if (tolower(symbol) == 'e' && 
                    expression[position + 1] == '-')
                {
                    number.push_back(symbol);
                    number.push_back(expression[position + 1]);
                    position += 2;
                    symbol = expression[position];
                }
                else
                {
                    number.push_back(symbol);
                    symbol = expression[++position];
                }
                
            }
            number.push_back('#');
            expressionOpz.append(number);
            continue;
        }

        // variavbles
        if (isalpha(symbol) || symbol == '_')
        {
            string name{ "" };
            while (isalnum(symbol) ||
                symbol == '_' &&
                position < expression.length())
            {
                name.push_back(symbol);
                symbol = expression[++position];
            }

            double value = variables[name];
            expressionOpz.append(to_string(value) + "#");
            continue;
        }

        // opening brackets
        if (bracketsOpen.find(symbol) != string::npos)
        {
            operatorsStack.push(symbol);
            continue;
        }

        // closing brackets
        if (bracketsClose.find(symbol) != string::npos)
        {
            while (!operatorsStack.empty() &&
                bracketsOpen.find(operatorsStack.top()) == string::npos)
            {
                expressionOpz.push_back(operatorsStack.top());
                operatorsStack.pop();
            }
            operatorsStack.pop();
            continue;
        }

        // multiplex operators
        if (operatorsMultiplex.find(symbol) != string::npos)
        {
            while (!operatorsStack.empty() &&
                operatorsMultiplex.find(operatorsStack.top()) != string::npos)
            {
                expressionOpz.push_back(operatorsStack.top());
                operatorsStack.pop();
            }
            operatorsStack.push(symbol);
        }

        // additive operators
        if (operatorsAdditive.find(symbol) != string::npos)
        {
            while (!operatorsStack.empty() &&
                bracketsOpen.find(operatorsStack.top()) != string::npos)
            {
                expressionOpz.push_back(operatorsStack.top());
                operatorsStack.pop();
            }
            operatorsStack.push(symbol);
        }
    }

    while (!operatorsStack.empty())
    {
        expressionOpz.push_back(operatorsStack.top());
        operatorsStack.pop();
    }
}
