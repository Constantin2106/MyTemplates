// MyDecorator.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include "src\Decorator.h"

int main()
{
    auto line = new SecondWord(new FirstWord(new BaseDecorator()));
    line->PrintWord();

    _getch();

    return 0;
}

