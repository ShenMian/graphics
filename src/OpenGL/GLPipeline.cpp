// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "GLPipeline.h"
#include "GLProgram.h"
#include <unordered_map>

namespace
{

std::unordered_map<CullMode, GLenum> GLCullMdoe = {{CullMode::Front, GL_FRONT}, {CullMode::Back, GL_BACK}};

std::unordered_map<PolygonMode, GLenum> GLPolygonMdoe = {{PolygonMode::Wireframe, GL_LINE},
                                                         {PolygonMode::Fill, GL_FILL}};

std::unordered_map<CompareOp, GLenum> GLCompareOp = {
    {CompareOp::AlwaysPass, GL_ALWAYS},   {CompareOp::NeverPass, GL_NEVER},  {CompareOp::Less, GL_LESS},
    {CompareOp::Equal, GL_EQUAL},         {CompareOp::LessEqual, GL_EQUAL},  {CompareOp::Greater, GL_GREATER},
    {CompareOp::GreaterEqual, GL_GEQUAL}, {CompareOp::NotEqual, GL_NOTEQUAL}};

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

void setViewports(const std::vector<GLViewport>& viewports)
{
	glViewportArrayv(1, viewports.size(), reinterpret_cast<const GLfloat*>(viewports.data()));
}

void setDepthRanges(const std::vector<GLDepthRange>& depthRanges)
{
	glDepthRangeArrayv(1, depthRanges.size(), reinterpret_cast<const GLdouble*>(depthRanges.data()));
}

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
	setupRasterizer(desc);
	setupViewports(desc);
	setupDepth(desc);
}

void GLPipeline::setupRasterizer(const Descriptor& desc)
{
	glPolygonMode(GL_FRONT_AND_BACK, GLPolygonMdoe[desc.rasterizer.polygonMode]);
	if(desc.rasterizer.cullMode != CullMode::Disabled)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GLCullMdoe[desc.rasterizer.cullMode]);
	}
	else
		glDisable(GL_CULL_FACE);
	glLineWidth(desc.rasterizer.lineWidth);
}

void GLPipeline::setupViewports(const Descriptor& desc)
{
	for(size_t i = 0; i < desc.viewports.size(); i++)
	{
		const auto& v = desc.viewports[i];
		glViewportIndexedf(i, v.x, v.y, v.width, v.height);
		glDepthRangeIndexed(i, v.minDepth, v.maxDepth);
	}
}

void GLPipeline::setupDepth(const Descriptor& desc)
{
	if(desc.depth.enableTest)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GLCompareOp[desc.depth.compareOp]);
	}
	else
		glDisable(GL_DEPTH_TEST);
	glDepthMask(desc.depth.enableWrite);
}
