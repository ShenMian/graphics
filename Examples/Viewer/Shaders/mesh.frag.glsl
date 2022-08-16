#version 450
#extension GL_KHR_vulkan_glsl : enable

// Copyright 2021 SMS
// License(Apache-2.0)

const float PI = 3.141592654;

struct Vert
{
	vec3 position;
	vec3 normal;
	vec2 tex_coord;
	vec3 tangent;
	vec3 bitangent;
	mat3 TBN;
};

layout(binding = 0) uniform Matrices
{
	mat4 view;
	mat4 proj;
	mat4 model;
} mat;

layout(location = 0) uniform sampler2D albedo;
layout(location = 1) uniform sampler2D metallic;
layout(location = 2) uniform sampler2D roughness;
layout(location = 3) uniform sampler2D ao;
layout(location = 4) uniform sampler2D emissive;
layout(location = 5) uniform sampler2D normal;

layout(location = 0) in Vert vert;

layout(location = 0) out vec4 frag_color;

void main()
{
	mat4 invView       = inverse(mat.view);
	vec3 cam_position  = invView[3].xyz;
	vec3 cam_direction = -normalize(invView[2].xyz);

	// if(dot(normalize(vert.position - cam_position), cam_direction) > 0.9)
	//     color.r = 1;

	vec4  albedo    = texture(albedo, vert.tex_coord).rgba;
	float metalness = texture(metallic, vert.tex_coord).r;
	float roughness = texture(roughness, vert.tex_coord).r;
	float ao        = texture(ao, vert.tex_coord).r;
	vec3  emissive  = texture(emissive, vert.tex_coord).rgb;

	if(albedo.a < 0.5)
		discard;

	frag_color = albedo;

	vec3 N = normalize(2.0 * texture(normal, vert.tex_coord).rgb - 1.0);
	N      = normalize(vert.TBN * vert.normal);

	vec3 direct_lighting = vec3(0.0);
}
