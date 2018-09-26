#include "Decorator.h"

void DecoratorTest()
{
   auto line = new SecondWord(new FirstWord(new BaseDecorator()));
   line->PrintWord();


   _getch();
}