// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

struct Vert
{
    vec2 tex_coord;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

layout(location = 0) out Vert vert;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vert.tex_coord = tex_coord;

    gl_Position = (projection * mat4(mat3(view)) * vec4(position, 1.0)).xyww;
}
