#pragma once

#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
    #define SYSTEM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    /* TARGET_OS_MAC exists on all the platforms
     * so we must check all of them (in this order)
     * to ensure that we're running on MAC
     * and not some other Apple platform */
    #if TARGET_IPHONE_SIMULATOR == 1
        #define SYSTEM_IOS_SIMULATOR
    #elif TARGET_OS_IPHONE == 1
        #define SYSTEM_IOS
    #elif TARGET_OS_MAC == 1
        #define SYSTEM_MACOS
    #endif
/*
    We also have to check __ANDROID__ before __linux__
    since android is based on the linux kernel
    it has __linux__ defined
*/
#elif defined(__ANDROID__)
    #define SYSTEM_ANDROID
#elif defined(__linux__)
    #define SYSTEM_LINUX
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
    #define SYSTEM_FREEBSD
#elif defined(__OpenBSD__)
    #define SYSTEM_OPENBSD
#elif defined(__NetBSD__)
    #define SYSTEM_NETBSD
#endif


#ifdef __clang__
    #define TOOLCHAIN_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define TOOLCHAIN_GCC
#elif defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    #define TOOLCHAIN_MSVC
#elif defined(__INTEL_COMPILER)
    #define TOOLCHAIN_INTEL
#endif


// The later ones are for MSVC
// GCC and CLANG user the former
#if defined __x86_64__ || defined _M_X64
    #define ARCHITECTURE_X64
#elif defined __i386__ || defined _M_IX86
    #define ARCHITECTURE_X86
#elif defined __arm__ || defined __arm64__ || defined _M_ARM
    #define ARCHITECTURE_ARM
#endif


#ifdef NDEBUG
    #define RELEASE
#else
    #define DEBUG
#endif