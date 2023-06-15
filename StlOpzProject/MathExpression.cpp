#include "MathExpression.h"

string& MathExpression::Expression()
{
    return expression;
}

string& MathExpression::ExpressionOpz()
{
    return expressionOpz;
}

int MathExpression::CheckBrackets()
{    
    const string brackets{ "()[]{}" };
    const string bracketsOpen{ "([{" };
    const string bracketsClose{ ")]}" };

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
