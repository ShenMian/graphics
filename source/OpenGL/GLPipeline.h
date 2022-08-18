// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../Pipeline.h"
#include <glad/glad.h>

class GLPipeline : public Pipeline
{
public:
	GLPipeline(const Descriptor& desc);
	~GLPipeline();

	void bind() override;

private:
	GLuint handle;
};
