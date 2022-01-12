#version 450

// Copyright 2021 SMS
// License(Apache-2.0)

layout (location = 0) out vec4 frag_color;

layout (location = 0) in Out
{
    vec3 position;
    vec3 color;
} vert;

void main()
{
    frag_color = vec4(vert.color, 0.1);
}
