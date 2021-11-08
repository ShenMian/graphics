// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out Out
{
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec3 tangent;
    vec3 bitangent;
} vert;

void main()
{
    vert.position  = position;
    vert.normal    = normal;
    vert.uv        = uv;
    vert.tangent   = tangent;
    vert.bitangent = bitangent;

    gl_Position = vec4(position, 1.0);
}