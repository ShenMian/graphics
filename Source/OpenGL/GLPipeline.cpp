// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLPipeline.h"
#include "GLProgram.h"
#include <unordered_map>

namespace
{

std::unordered_map<CullMode, GLenum> GLCullMdoe = {{CullMode::Front, GL_FRONT}, {CullMode::Back, GL_BACK}};

std::unordered_map<PolygonMode, GLenum> GLPolygonMdoe = {{PolygonMode::Wireframe, GL_LINE},
                                                         {PolygonMode::Fill, GL_FILL}};

} // namespace

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

	desc.program->use();

	glPolygonMode(GL_FRONT_AND_BACK, GLPolygonMdoe[desc.rasterizer.polygonMode]);
	if(desc.rasterizer.cullMode != CullMode::Disabled)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GLCullMdoe[desc.rasterizer.cullMode]);
	}
	else
		glDisable(GL_CULL_FACE);
}
