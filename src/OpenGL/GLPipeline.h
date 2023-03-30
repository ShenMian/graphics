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
	void setup_rasterizer(const Descriptor& desc);
	void setup_viewports(const Descriptor& desc);
	void setup_depth(const Descriptor& desc);

	GLuint handle;
};
