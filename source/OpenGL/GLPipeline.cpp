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

struct GLViewport
{
	GLfloat x;
	GLfloat y;
	GLfloat width;
	GLfloat height;
};

struct GLDepthRange
{
	GLdouble minDepth;
	GLdouble maxDepth;
};

} // namespace

GLPipeline::GLPipeline(const Descriptor& desc) : Pipeline(desc)
{
	GLint maxViewports = 0;
	glGetIntegerv(GL_MAX_VIEWPORTS, &maxViewports);

	if(desc.viewports.size() >= maxViewports)
		throw std::runtime_error("too many viewports");

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

	std::vector<GLViewport>   viewports;
	std::vector<GLDepthRange> depthRanges;
	for(const auto& viewport : desc.viewports)
	{
		viewports.push_back({viewport.x, viewport.y, viewport.width, viewport.height});
		depthRanges.push_back({viewport.minDepth, viewport.maxDepth});
	}
	glViewportArrayv(1, viewports.size(), reinterpret_cast<const GLfloat*>(viewports.data()));
	glDepthRangeArrayv(1, depthRanges.size(), reinterpret_cast<const GLdouble*>(depthRanges.data()));
}
