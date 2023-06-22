#include <iostream>
#include "MathExpression.h"

int main()
{
    //MathExpression expr("(a + 20) / b - (34 + 15e-1) / c");
    MathExpression expr("5 + sin(2)");
    expr["a"] = 10;
    expr["b"] = 4.5;
    expr["c"] = 123e-2;

    expr.OpzCreate();
    cout << expr.ExpressionOpz();
    std::cout << "\n";

    cout << expr.OpzCalculate();
}
