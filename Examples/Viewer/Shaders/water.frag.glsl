// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

const float PI = 3.141592654;

layout (location = 0) out vec4 frag_color;

uniform float time;

uniform sampler2D noise;

void main()
{
	frag_color = vec4(0.0);
}
