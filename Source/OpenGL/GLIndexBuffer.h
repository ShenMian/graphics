// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../IndexBuffer.h"
#include <glad/glad.h>

class GLIndexBuffer : public IndexBuffer
{
public:
    using handle_type = unsigned int;

    GLIndexBuffer(const void* data, size_t size, size_t count, Usage usage);
    virtual ~GLIndexBuffer();

    void bind() override;

private:
    handle_type handle;
};