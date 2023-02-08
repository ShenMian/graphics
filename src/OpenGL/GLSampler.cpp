// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "GLSampler.h"
#include <unordered_map>

namespace
{

std::unordered_map<Sampler::Filter, GLenum> GLFilter = {{Sampler::Filter::Nearest, GL_NEAREST},
                                                        {Sampler::Filter::Bilinear, GL_LINEAR},
                                                        {Sampler::Filter::Trilinear, GL_LINEAR_MIPMAP_LINEAR}};

std::unordered_map<Sampler::Warp, GLenum> GLWarp = {{Sampler::Warp::Repeat, GL_REPEAT},
                                                    {Sampler::Warp::MirrorRepeat, GL_MIRRORED_REPEAT},
                                                    {Sampler::Warp::ClampToEdge, GL_CLAMP_TO_EDGE}};

} // namespace

GLSampler::GLSampler(const Descriptor& desc) : Sampler(desc)
{
	glCreateSamplers(1, &handle);

	glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, GLFilter[minFilter]);
	glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, GLFilter[magFilter]);
	glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, GLWarp[SWarp]);
	glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, GLWarp[TWarp]);
	glSamplerParameteri(handle, GL_TEXTURE_WRAP_R, GLWarp[RWarp]);

	/*
	GLfloat maxAniso;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	if(v > maxAniso)
	    throw std::runtime_error("");

	glSamplerParameterf(handle, GL_TEXTURE_MAX_ANISOTROPY_EXT, v);
	*/
}

GLSampler::~GLSampler()
{
	glDeleteSamplers(1, &handle);
}
