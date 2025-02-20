#pragma once

#include <cstdarg>
#include <iostream>

namespace Utils{

template <typename T>
static void print(T t) 
{
    std::cout << t << " " ;
}

template<typename... Args>
void print(Args... args)
{

    print(args...) ;
    std::cout << std::endl;
}


}