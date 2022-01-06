// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out Out
{
    vec3 position;
    vec3 color;
} vert;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vert.position = position;
    vert.color    = color;

    gl_Position = projection * view * model * vec4(position, 1.0);
}
