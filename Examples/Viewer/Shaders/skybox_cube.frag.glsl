// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

struct Vert
{
    vec2 tex_coord;
};

const float PI = 3.141592654;

layout(location = 0) in Vert vert;

layout(location = 0) out vec4 frag_color;

uniform samplerCube cubemap;

void main()
{
    frag_color = texture(cubemap, vert.tex_coord);
}
