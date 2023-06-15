#include <iostream>
#include "MathExpression.h"

int main()
{
    MathExpression expr("");
    cout << expr.CheckBrackets();

    expr["a"] = 10;
    expr["b"] = 5.4;
    expr["a"] = 20;
}
