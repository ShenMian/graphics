// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLVertexArray.h"
#include "../VertexFormat.h"
#include "GLCheck.h"
#include <glad/glad.h>
#include <unordered_map>

namespace
{

std::unordered_map<Format, GLenum> GLType = {
	{Format::R16F, GL_HALF_FLOAT},
	{Format::RG16F, GL_HALF_FLOAT},
	{Format::RGB16F, GL_HALF_FLOAT},
	{Format::RGBA16F, GL_HALF_FLOAT},
	{Format::R32F, GL_FLOAT},
	{Format::RG32F, GL_FLOAT},
	{Format::RGB32F, GL_FLOAT},
	{Format::RGBA32F, GL_FLOAT},
};

// TODO: FormatAttribute
std::unordered_map<Format, GLenum> Components = {
	{Format::R16F, 1},
	{Format::RG16F, 2},
	{Format::RGB16F, 3},
	{Format::RGBA16F, 4},
	{Format::R32F, 1},
	{Format::RG32F, 2},
	{Format::RGB32F, 3},
	{Format::RGBA32F, 4},
};

}

GLVertexArray::GLVertexArray()
{
	glCreateVertexArrays(1, &handle);
}

GLVertexArray::~GLVertexArray()
{
	glDeleteVertexArrays(1, &handle);
}

void GLVertexArray::bind()
{
	glBindVertexArray(handle);
}

void GLVertexArray::build(const VertexFormat& fmt)
{
	bind();
	for(const auto& attr : fmt.getAttributes())
	{
		glEnableVertexAttribArray(attribIndex);
		glVertexAttribPointer(attribIndex, Components[attr.format], GLType[attr.format], attr.normalized,
			static_cast<GLsizei>(fmt.getStride()),
			reinterpret_cast<const void*>(attr.offset));
		attribIndex++;
		GLCheckError();
	}
	// glVertexAttribDivisor
}
