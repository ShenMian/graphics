// Copyright 2022 ShenMian
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
	void setupRasterizer(const Descriptor& desc);
	void setupViewports(const Descriptor& desc);
	void setupDepth(const Descriptor& desc);

	GLuint handle;
};
