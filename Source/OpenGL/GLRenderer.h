// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../Renderer.h"

class GLRenderer : public Renderer
{
public:
    std::string getDeviceName() const override;
    std::string getRendererName() const override;
    std::string getVendorName() const override;
};

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