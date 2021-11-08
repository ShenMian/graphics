// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

const float PI = 3.141592654;

layout (location = 0) out vec4 frag_color;

in Out
{
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec3 tangent;
    vec3 bitangent;
} vert;

void main()
{
    frag_color = vec4(1.0);
}