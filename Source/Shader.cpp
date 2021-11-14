// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Shader.h"
#include "Renderer.h"
#include <cassert>

#include "OpenGL/GLShader.h"
#include "Vulkan/VKShader.h"

std::unordered_map<Shader::Stage, const char*> Shader::extension = {
    {Shader::Stage::Vertex, "vert"},
    {Shader::Stage::Geometry, "geom"},
    {Shader::Stage::Fragment, "frag"},
    {Shader::Stage::Compute, "comp"}};

std::shared_ptr<Shader> Shader::create(const std::string& name, Stage stage)
{
    switch(Renderer::getAPI())
    {
        using enum Renderer::API;

    case OpenGL:
        return std::make_shared<GLShader>(name, stage);

    case Vulkan:
        return std::make_shared<VKShader>(name, stage);

    default:
        assert(false);
    }
    return nullptr;
}

Shader::Shader(const std::string& name, Stage stage)
    : name(name), stage(stage)
{
}

const std::string& Shader::getName() const
{
    return name;
}

Shader::Stage Shader::getStage() const
{
    return stage;
}