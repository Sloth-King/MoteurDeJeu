#pragma once

#include <cstdarg>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <chrono>
#include <string>
namespace Utils
{

    template <typename T>
    static void print(T t)
    {
        std::cout << t << " ";
    }

    template <typename... Args>
    void print(Args... args)
    {

        print(args...);
        std::cout << std::endl;
    }

    inline int posmod(int i, int n)
    {
        return (i % n + n) % n;
    }

    template <const char name>
    inline void time(const bool restart = false, const std::string msg = "")
    {
        static bool started = false;
        static std::chrono::steady_clock::time_point last_time;

        std::chrono::steady_clock::time_point new_time = std::chrono::steady_clock::now();

        if (!started or restart)
        {
            last_time = new_time;
            started = true;
            return;
        }
        std::cout << "Delta for timer '" << name << "' : " << std::chrono::duration_cast<std::chrono::milliseconds>(new_time - last_time).count() << "ms";
        if (msg != "")
            std::cout << " (with message '" << msg << "')";

        std::cout << std::endl;
    }

}