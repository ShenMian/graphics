// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

void GLCheckError();
void GLClearError();

#if 1
#    define GLCall(func)    \
        do                  \
        {                   \
            GLClearError(); \
            func;           \
            GLCheckError(); \
        } while(false)
#else
#    define GLCall(func) func
#endif
