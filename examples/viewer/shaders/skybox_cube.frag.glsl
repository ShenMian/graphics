// Copyright 2022 ShenMian
// License(Apache-2.0)

#version 450

struct Vert
{
    vec2 tex_coord;
};

const float PI = 3.14159265359;

layout(location = 0) in Vert vert;

layout(location = 0) out vec4 frag_color;

uniform samplerCube cube_map;

void main()
{
    frag_color = texture(cube_map, vert.tex_coord);
}
