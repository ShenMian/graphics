#version 450

// Copyright 2021 SMS
// License(Apache-2.0)

layout(binding = 0) uniform Matrices
{
    mat4 view;
    mat4 proj;
    mat4 model;
} mat;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 v_color;

void main()
{
    v_color  = color;
    gl_Position = mat.proj * mat.view * mat.model * vec4(position, 1.0);
}
