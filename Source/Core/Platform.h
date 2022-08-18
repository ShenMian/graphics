// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

// OS

#define OS_UNKNOWN 0
#define OS_WIN 1
#define OS_LINUX 2
#define OS_MAC 3
#define OS_ANDROID 4
#define OS_IOS 5

#if defined(_WIN32)
#define TARGET_OS OS_WIN
#elif defined(__linux__) && !defined(__ANDROID__)
#define TARGET_OS OS_LINUX
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE // TARGET_OS_IPHONE includes TARGET_OS_IOS TARGET_OS_TV and TARGET_OS_WATCH. see
                     // TargetConditionals.h
#define TARGET_OS OS_IOS
#elif TARGET_OS_MAC
#define TARGET_OS OS_MAC
#endif
#elif defined(__ANDROID__)
#define TARGET_OS OS_ANDROID
#else
#define TARGET_OS OS_UNKNOWN
#endif

#if TARGET_OS == OS_UNKNOWN
#error "The target platform is not supported"
#endif

// Compiler

#define COMPILER_UNKNOWN 0
#define COMPILER_MSVC 1
#define COMPILER_CLANG 2
#define COMPILER_GCC 3

#if defined(_MSC_VER)
#define TARGET_COMPILER COMPILER_MSVC
#elif defined(__clang__)
#define TARGET_COMPILER COMPILER_CLANG
#elif defined(__GNUC__)
#define TARGET_COMPILER COMPILER_GCC
#else
#define TARGET_COMPILER COMPILER_UNKNOWN
#endif
