﻿// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLIndexBuffer.h"
#include <cstring>
#include <unordered_map>

#include "../Renderer.h"

namespace
{

std::unordered_map<Buffer::Usage, uint32_t> GLUsage = {{Buffer::Usage::Static, GL_STATIC_DRAW},
                                                       {Buffer::Usage::Dynamic, GL_DYNAMIC_DRAW},
                                                       {Buffer::Usage::Stream, GL_STREAM_DRAW}};

}

GLIndexBuffer::GLIndexBuffer(const uint32_t* data, size_t size, Buffer::Usage usage)
    : IndexBuffer(data, size), buffer(size, Buffer::Type::Index, usage)
{
	buffer.map();
	buffer.write(data, size);
	buffer.unmap();
}

Buffer& GLIndexBuffer::getBuffer()
{
	return buffer;
}

void GLIndexBuffer::bind()
{
	buffer.bind();
}
