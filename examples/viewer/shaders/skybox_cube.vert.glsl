// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

struct Vert
{
    vec2 tex_coord;
};

layout(binding = 0) uniform Matrices
{
    mat4 view;
    mat4 proj;
    mat4 model;
} mat;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

layout(location = 0) out Vert vert;

void main()
{
    vert.tex_coord = tex_coord;
    gl_Position = (mat.proj * mat4(mat3(mat.view)) * vec4(position, 1.0)).xyww;
}
