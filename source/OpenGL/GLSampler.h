// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Sampler.h"
#include <glad/glad.h>

class GLSampler : public Sampler
{
public:
	GLSampler(const Descriptor& desc);
	~GLSampler();

private:
	GLuint handle;
};
