#pragma once
#include <string>
#include <cstdio>
#include <cstring>
#include <utility>
#include <iostream>
#include <algorithm>

#include "tinyfiledialogs/tinyfiledialogs.h"


template <typename... Args>
inline std::string Format(const char* fmt, Args&&... args) noexcept
{
    size_t size = (size_t)std::snprintf(NULL, 0, fmt, std::forward<Args>(args)...) + 1; // Extra space for '\0'
    std::string res(size, 0);
    std::snprintf(res.data(), size, fmt, std::forward<Args>(args)...);
    res.pop_back(); // '\0'
    return res;
}


template <class... Args>
inline void Log(const char* fmt, Args&&... args)
{
    std::cout << "[INFO] " << Format(fmt, std::forward<Args>(args)...) << std::endl;
}


template <class... Args>
inline void Err(const char* fmt, Args&&... args)
{
    const std::string msg = Format(fmt, std::forward<Args>(args)...);
    std::cerr << "[ERROR] " << msg << std::endl;
    tinyfd_messageBox("Error", msg.c_str(), "ok", "error", 1);
}