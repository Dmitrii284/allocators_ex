#include <iostream>
#include <vector>

#include "DinamicArray.hpp"

class A {
public:
    void f00()const {
    }
};

template<typename T>
void f01(T a){ }

int main()
{
    f01(1);
    f01<int>(.1);
    extern DinamicArray<int> xyz3;
    std::cout << "Hello World!\n";
    std::vector<int> abc1;
    std::vector<A> abc2(31);
    std::vector<int> abc3{12313,234234,1231231};
    std::vector<int> abc4{xyz3.begin(),xyz3.end()};
    abc1.front();
    abc1.back();
    abc1.push_back(23);
    int a = abc1.capacity();
    int b = abc1.size();

    DinamicArray<int> xyz1;
    DinamicArray<A> xyz2(12);

    DinamicArray<A>::iterator tmp = xyz2.begin();
    A *pTmp = new A;
    pTmp->f00();
    tmp->f00();
    A t;
    //A();
    DinamicArray<int> xyz3{123,123,121};
    DinamicArray<int> xyz4{abc3.begin().,abc3.end()};
}