#include <iostream>
#include <vector>

#include "Array.hpp"




int main()
{
    std::cout << "Hello World!\n";
    std::vector<int> abc(1000);
    abc.push_back(23);
    auto a = abc.capacity();
    auto b =abc.size();
}