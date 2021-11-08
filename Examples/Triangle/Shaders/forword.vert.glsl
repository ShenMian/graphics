// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

const float PI = 3.141592654;

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out vec3 v_color;

void main()
{
    v_color = color;
    gl_Position = vec4(position, 0.0, 1.0);
}