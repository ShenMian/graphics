// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

layout(binding = 0) uniform sampler2D albedo_map;

layout(location = 0) in vec2 tex_coord;

layout(location = 0) out vec4 frag_color;

void main()
{
	vec3 color = vec3(1.0, 0.0, 0.0);
	frag_color = vec4(color, 1.0) * texture(albedo_map, tex_coord).r;
}
