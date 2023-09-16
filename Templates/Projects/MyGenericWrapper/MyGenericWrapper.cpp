// MyGenericWrapper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

#include "Holder.h"
#include "GenericWrapper.h"

using namespace std;

namespace Validators 
{
    bool IsPositive(const int value)
    {
        return value >= 0;
    }
    bool IsNotEmpty(const std::vector<int> container)
    {
        return !container.empty();
    }
}

int main()
{
    std::cout << "Hello World!\n";

    Holder number(0, &Validators::IsPositive);
    auto i = ++number.get();
    cout << "i = " << i << endl;
    const int n = number.get();
    cout << endl;

    Holder<vector<int>> container(Validators::IsNotEmpty);
    container->push_back(100);
    container.get().push_back(200);
    const auto size = container->size();
    const auto first = container[0];
    cout << "container size = " << size << endl;
    cout << "first value = " << first << endl;

    Wrapper<vector<int>> wrapper(Validators::IsNotEmpty);
    wrapper->push_back(100);

}
