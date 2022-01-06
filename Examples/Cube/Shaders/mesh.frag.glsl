// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

layout (location = 0) out vec4 frag_color;

in Out
{
    vec3 position;
    vec3 color;
} vert;

void main()
{
    frag_color = vec4(vert.color, 0.1);
}
