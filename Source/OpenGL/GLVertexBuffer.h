// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include "../VertexBuffer.h"
#include <glad/glad.h>

class GLVertexBuffer : public VertexBuffer
{
public:
    using handle_type = unsigned int;

    GLVertexBuffer(const void* data, size_t size, size_t count, Usage usage);
    virtual ~GLVertexBuffer();

    void bind() override;

private:
    handle_type handle;
};