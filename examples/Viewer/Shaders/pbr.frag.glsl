// Copyright 2021 SMS
// License(Apache-2.0)

#version 450
#extension GL_KHR_vulkan_glsl : enable
precision highp float;

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

struct DirectionalLight
{
    vec3  color;
    float intesity;

    vec3 direction;
};

struct PointLight
{
    vec3  color;
    float intesity;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3  color;
    float intesity;

    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

layout(binding = 0) uniform Matrices
{
	mat4 view;
	mat4 proj;
	mat4 model;
} mat;

layout(location = 0) uniform sampler2D albedo_map;
layout(location = 1) uniform sampler2D normal_map;
layout(location = 2) uniform sampler2D metallic_map;
layout(location = 3) uniform sampler2D roughness_map;
layout(location = 4) uniform sampler2D emissive_map;
layout(location = 5) uniform sampler2D occlusion_map;

layout(location = 0) in Vert vert;

layout(location = 0) out vec4 frag_color;

vec3 flat_normal(vec3 pos) {
	vec3 fdx = dFdx(pos);
	vec3 fdy = dFdy(pos);

	return normalize(cross(fdx, fdy));
}

mat3 get_tbn_matrix()
{
	vec3 q1  = dFdx(vert.position);
	vec3 q2  = dFdy(vert.position);
	vec2 st1 = dFdx(vert.tex_coord);
	vec2 st2 = dFdy(vert.tex_coord);

	vec3 N = normalize(vert.normal);
	vec3 T = normalize(q1 * st2.t - q2 * st1.t);
	vec3 B = -normalize(cross(N, T));

	return mat3(T, B, N);
}

vec3 get_normal()
{
	vec3 tangent_normal = normalize(texture(normal_map, vert.tex_coord).rgb * 2.0 - 1.0);
	mat3 TBN            = get_tbn_matrix();

	return TBN * tangent_normal;
}

void main()
{
	mat4 inv_view = inverse(mat.view);
	vec3 cam_pos  = inv_view[3].xyz;
	vec3 cam_dir  = -normalize(inv_view[2].xyz);

	vec4  albedo    = texture(albedo_map, vert.tex_coord).rgba;
	vec3  normal    = get_normal();
	float metalness = texture(metallic_map, vert.tex_coord).r;
	float roughness = texture(roughness_map, vert.tex_coord).r;
	vec3  emissive  = texture(emissive_map, vert.tex_coord).rgb;
	float occlusion = texture(occlusion_map, vert.tex_coord).r;

	frag_color = albedo;

	if(vec3(albedo) == vec3(0.0))
		frag_color = vec4(vert.tex_coord, 0.0, 1.0);
		
	// frag_color = vec4(texture(normal_map, vert.tex_coord).rgb, 1.0);

	// frag_color = vec4(normal, 1.0);              // DEBUG: normal only
	// frag_color = vec4(emissive, 1.0);            // DEBUG: emissive only
	// frag_color = vec4(vec3(metalness), 1.0);     // DEBUG: metalness only
	// frag_color = vec4(vec3(roughness), 1.0);     // DEBUG: roughness only
	// frag_color = vec4(vec3(occlusion), 1.0);     // DEBUG: occlusion only
	// frag_color = vec4(vert.tex_coord, 0.0, 1.0); // DEBUG: uv only
}
