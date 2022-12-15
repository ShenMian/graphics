// Copyright 2022 ShenMian
// License(Apache-2.0)

#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 v_color;

void main()
{
    v_color = color;
    gl_Position = vec4(position, 0.0, 1.0);
}
