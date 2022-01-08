// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out Out
{
    vec3 position;
    vec3 normal;
    vec2 tex_coord;
    vec3 tangent;
    vec3 bitangent;
} vert;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vert.position  = vec3(model * vec4(position, 1.0));
    vert.normal    = normalize(mat3(transpose(inverse(model))) * normal);
    vert.tex_coord = tex_coord;
    vert.tangent   = tangent;
    vert.bitangent = bitangent;

    gl_Position = projection * view * vec4(vert.position, 1.0);
}
