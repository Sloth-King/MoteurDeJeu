#pragma once

#include <cstdarg>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

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

inline int posmod(int i, int n) {
    return (i % n + n) % n;
}


}