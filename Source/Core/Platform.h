// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#define PLATFORM_UNKNOWN 0
#define PLATFORM_WIN     1
#define PLATFORM_LINUX   2
#define PLATFORM_MAC     3
#define PLATFORM_ANDROID 4
#define PLATFORM_IOS     5

#ifdef _WIN32
	#define TARGET_PLATFORM PLATFORM_WIN
#elif __linux__
	#define TARGET_PLATFORM PLATFORM_LINUX
#elif __APPLE__
	#include <TargetConditionals.h>
	#if TARGET_OS_IPHONE // TARGET_OS_IPHONE includes TARGET_OS_IOS TARGET_OS_TV and TARGET_OS_WATCH. see TargetConditionals.h
		#define TARGET_PLATFORM PLATFORM_IOS
	#elif TARGET_OS_MAC
		#define TARGET_PLATFORM PLATFORM_MAC
	#endif
#elif ANDROID
	#define TARGET_PLATFORM PLATFORM_ANDROID
#else
	#define TARGET_PLATFORM PLATFORM_UNKNOWN
#endif

#if TARGET_PLATFORM == PLATFORM_UNKNOWN
	#error "The target platform is not supported"
#endif
