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

    string fkeys{ "" };
    string fvals{ "" };
    for (auto f : this->functions)
    {
        fkeys.push_back(f.first);
        fvals += f.second;
    }

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
            position--;
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

            // name is functions name
            if (fvals.find(name) != string::npos)
            {
                operatorsStack.push(name[0]);
            }
            else
            {
                double value = variables[name];
                expressionOpz.append(to_string(value) + "#");
            }
            position--;
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
                (operatorsMultiplex.find(operatorsStack.top()) != string::npos ||
                fkeys.find(operatorsStack.top()) != string::npos))
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
                (operatorsMultiplex.find(operatorsStack.top()) != string::npos ||
                operatorsAdditive.find(operatorsStack.top()) != string::npos ||
                fkeys.find(operatorsStack.top()) != string::npos))
            {
                
                expressionOpz.push_back(operatorsStack.top());
                operatorsStack.pop();
            }
            operatorsStack.push(symbol);
        }
    }

    while (!operatorsStack.empty())
    {
        if (bracketsOpen.find(operatorsStack.top()) == string::npos)
            expressionOpz.push_back(operatorsStack.top());
        operatorsStack.pop();
    }
}

double MathExpression::OpzCalculate()
{
    stack<double> operandsStack;

    string fkeys{ "" };
    for (auto f : this->functions)
        fkeys.push_back(f.first);

    
    int position{};
    while (position < expressionOpz.length())
    {
        char symbol = expressionOpz[position];

        if (isdigit(symbol) || symbol == '.')
        {
            string operand = "";

            while (symbol != '#')
            {
                operand.push_back(symbol);
                symbol = expressionOpz[++position];
            }
            operandsStack.push(stof(operand));
            position++;
            continue;
        }

        if ((operatorsMultiplex.find(symbol) != string::npos) ||
            operatorsAdditive.find(symbol) != string::npos)
        {
            double operand2 = operandsStack.top();
            operandsStack.pop();
            double operand1 = operandsStack.top();
            operandsStack.pop();
            
            double result;
            switch (symbol)
            {
            case '*': result = operand1 * operand2; break;
            case '/': result = operand1 / operand2; break;
            case '+': result = operand1 + operand2; break;
            case '-': result = operand1 - operand2; break;
            default:
                break;
            }
            operandsStack.push(result);
            position++;
            continue;
        }

        if (fkeys.find(symbol) != string::npos)
        {
            double operand = operandsStack.top();
            operandsStack.pop();

            switch (symbol)
            {
            case 'c': operandsStack.push(cos(operand)); break;
            case 's': operandsStack.push(sin(operand)); break;
            case 't': operandsStack.push(tan(operand)); break;
            case 'a': operandsStack.push(1/tan(operand)); break;
            default:
                break;
            }
            position++;
            continue;
        }
    }
    return operandsStack.top();
}

double MathExpression::Calculate()
{
    if (this->CheckBrackets() == -1)
    {
        try 
        {
            return this->OpzCalculate();
        }
        catch (...)
        {
            throw exception("error at calculate");
        }
        
    }
    else
        throw exception("error at brackets...");
}
