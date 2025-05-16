#pragma once

#include <cstdarg>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <chrono>

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
/*
template<const char* name>
inline float time(restart = false){
    static std::chrono::steady_clock::time_point last_time = -1;

    std::chrono::steady_clock::time_point new_time = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000.0 << "ms" << std::endl;

}
*/



}