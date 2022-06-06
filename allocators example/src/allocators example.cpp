#include <iostream>
#include <vector>

#include "DinamicArray.hpp"




int main()
{
    extern DinamicArray<int> xyz3;
    std::cout << "Hello World!\n";
    std::vector<int> abc1;
    std::vector<int> abc2(31);
    std::vector<int> abc3{12313,234234,1231231};
    std::vector<int> abc4{xyz3.begin(),xyz3.end()};
    abc1.front();
    abc1.back();
    abc1.push_back(23);
    auto a = abc1.capacity();
    auto b = abc1.size();

    DinamicArray<int> xyz1;
    DinamicArray<int> xyz2(12);
    DinamicArray<int> xyz3{123,123,121};
    DinamicArray<int> xyz4{abc3.begin().,abc3.end()};
}