#version 450

// Copyright 2021 SMS
// License(Apache-2.0)

struct Vert
{
    vec3 color;
};

layout(location = 0) in Vert vert;

layout(location = 0) out vec4 frag_color;

void main()
{
    frag_color = vec4(vert.color, 0.1);
}
