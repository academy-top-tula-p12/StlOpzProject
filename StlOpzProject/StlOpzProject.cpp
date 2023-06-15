#include <iostream>
#include "MathExpression.h"

int main()
{
    MathExpression expr("10 * (20 + 30)"); // 10#20#30#+*
    //cout << expr.CheckBrackets();

    expr.OpzCreate();
    cout << expr.ExpressionOpz();

    /*expr["a"] = 10;
    expr["b"] = .4;
    expr["a"] = 20;*/
}
