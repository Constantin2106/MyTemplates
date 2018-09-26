#include <conio.h>
#include <iostream>

#include "Callback.h"


int X[]{ 0, 1, 2, 3, 4 };

int Add(int& x)
{
    x += 1;
    return x;
}
int Dev(int& x)
{
   x -= 1;
   return x;
}
double Mul(int& x)
{
    x *= 10;
    return x;
}


void ProcessElementFunction(unsigned idx, FuncRef<int(int&)> func)
{
    int y = func(X[idx]);
}
void ProcessElementLambda(unsigned idx, LambdaRef<double(int&)> lambda)
{
    double y = lambda(X[idx]);
}

void LambdaTest()
{
    FuncRef<int(int&)> add = &Add;
    const FuncRef<double(int&)> mul = &Mul;

    for(unsigned i = 0; i < 5; i++)
    {
        std::cout << "Before: " << X[i];

        /*ProcessElementLambda(i, [&](int& x) -> double
        {
            x++;
            return x;
        });*/

        ProcessElementFunction(i, add);
        //ProcessElementFunction(i, Mul);

        std::cout << "   After: " << X[i] << std::endl;
    }

    _getch();
}