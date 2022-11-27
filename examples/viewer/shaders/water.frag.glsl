// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

const float PI = 3.14159265359;

uniform float time;

layout(location = 0) uniform sampler2D noise;

layout(location = 0) out vec4 frag_color;

void main()
{
	frag_color = vec4(0.0);
}
