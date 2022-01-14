#version 450

// Copyright 2021 SMS
// License(Apache-2.0)

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

layout (location = 0) out Out
{
    vec3 position;
    vec3 color;
} vert;

/*
layout (std140, binding = 0) uniform Uniform
{
    mat4 model;
    mat4 view;
    mat4 projection;
};
*/

out gl_PerVertex
{
    vec4 gl_Position;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vert.position = position;
    vert.color    = color;

    gl_Position = projection * view * model * vec4(position, 1.0);
}
