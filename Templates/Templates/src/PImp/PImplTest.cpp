#include <conio.h>
#include <iostream>

#include "PImp.h"

void PImplTest()
{
    std::cout << "Create PImpl" << std::endl;

    PImpl pimpl;

    pimpl.Set();
    pimpl.Check();
    pimpl.Execute();

    _getch();

    //delete pimpl;

}