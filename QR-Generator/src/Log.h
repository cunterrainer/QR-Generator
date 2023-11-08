#pragma once
#include <string>
#include <cstdio>
#include <utility>
#include <iostream>

#include "Core.h"

#ifdef SYSTEM_WINDOWS
    #include <Windows.h>
    #undef max // windows macros
    #undef min // windows macros
#elif defined(SYSTEM_LINUX)
    #include <locale>
    #include <sstream>
    #include "MsgBoxX11/MsgBoxX11.h"
#endif


#ifdef SYSTEM_WINDOWS
    inline int MsgBoxError(const char* message) { return MessageBoxA(NULL, message, "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL); }
#elif defined(SYSTEM_LINUX)
    inline std::wstring WidenString(const char* str)
    {
        const size_t length = strlen(str);
        std::wstring result;
        result.resize(length);

        const auto& facet = std::use_facet<std::ctype<wchar_t>>(std::wostringstream().getloc());
        std::transform(str, str + length, result.begin(), [&facet](const char ch)
        {
            return facet.widen(ch);
        });
        return result;
    }

    int MsgBoxError(const char* message)
    {
        Button btn;
        btn.label = L"Ok";
        btn.result = 1;
        return Messagebox("Error", WidenString(message).c_str(), &btn, 1);
    }
#elif defined(SYSTEM_MACOS)
    extern "C" void MessageBoxErrorMacOS(const char* message);
    inline int MsgBoxError(const char* message)
    {
        MessageBoxErrorMacOS(message);
        return 0;
    }
#endif


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
    MsgBoxError(msg.c_str());
}