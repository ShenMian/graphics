// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

const float PI = 3.141592654;

layout (location = 0) out vec4 frag_color;

out Out
{
    vec2 tex_coord;
} vert;

uniform samplerCube cubemap;

void main()
{
    frag_color = texture(cubemap, vert.tex_coord);
}
