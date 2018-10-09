#pragma once
#include <conio.h>
#include <iostream>

class BaseDecorator
{
public:
   BaseDecorator();
   ~BaseDecorator();

   virtual void PrintWord()
   {
      std::cout << "Output: ";
   }

private:
};

class FirstWord : public BaseDecorator
{
public:
   FirstWord(BaseDecorator* decor) : mDecor(decor)
   {
   }
    
   void PrintWord() override
   {
      if(mDecor)
         mDecor->PrintWord();
      std::cout << "first ";
   }

private:
   BaseDecorator* mDecor;
};

class SecondWord : public BaseDecorator
{
public:
   SecondWord(BaseDecorator* decor) : mDecor(decor)
   {
   }

   void PrintWord() override
   {
      if(mDecor)
         mDecor->PrintWord();
      std::cout << "second ";
   }

private:
   BaseDecorator* mDecor;
};

