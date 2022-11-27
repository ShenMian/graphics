// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

layout(binding = 0) uniform Matrices
{
    mat4 view;
    mat4 proj;
    mat4 model;
} mat;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 tex_coord;

layout(location = 0) out vec2 out_tex_coord;

void main()
{
    out_tex_coord = tex_coord;
    // gl_Position = mat.proj * vec4(position.x, position.y, 0.0, 1.0);
    // gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    gl_Position = vec4(position.x * 0.01, position.y * 0.01, 0.0, 1.0);
}
