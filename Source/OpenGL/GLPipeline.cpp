// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLPipeline.h"

GLPipeline::GLPipeline(const Descriptor& desc) : Pipeline(desc)
{
	glCreateProgramPipelines(1, &handle);
}

GLPipeline::~GLPipeline()
{
	glDeleteProgramPipelines(1, &handle);
}

void GLPipeline::bind()
{
	glBindProgramPipeline(handle);
}
