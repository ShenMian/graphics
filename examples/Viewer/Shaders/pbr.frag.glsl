// Copyright 2021 SMS
// License(Apache-2.0)

#version 450
#extension GL_KHR_vulkan_glsl : enable
precision highp float;

const float PI = 3.14159265359;

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

#define DEBUG_ONLY_NORMAL    0
#define DEBUG_ONLY_OCCLUSION 0

vec3 flat_normal(vec3 pos) {
	const vec3 fdx = dFdx(pos);
	const vec3 fdy = dFdy(pos);

	return normalize(cross(fdx, fdy));
}

mat3 get_tbn_matrix()
{
	const vec3 q1  = dFdx(vert.position);
	const vec3 q2  = dFdy(vert.position);
	const vec2 st1 = dFdx(vert.tex_coord);
	const vec2 st2 = dFdy(vert.tex_coord);

	const vec3 N = normalize(vert.normal);
	const vec3 T = normalize(q1 * st2.t - q2 * st1.t);
	const vec3 B = -normalize(cross(N, T));

	return mat3(T, B, N);
}

vec3 get_normal()
{
	const vec3 tangent_normal = normalize(texture(normal_map, vert.tex_coord).rgb * 2.0 - 1.0);
	const mat3 TBN            = get_tbn_matrix();

	return TBN * tangent_normal;
}

float distribution_GGX(vec3 N, vec3 H, float roughness)
{
    const float a  = roughness * roughness;
    const float a2 = a * a;
    const float NdotH  = max(dot(N, H), 0.0);
    const float NdotH2 = NdotH * NdotH;

    const float nom = a2;
    float denom     = (NdotH2 * (a2 - 1.0) + 1.0);
    denom           = PI * denom * denom;

    return nom / denom;
}

float geometry_schlick_GGX(float NdotV, float roughness)
{
    const float r = roughness + 1.0;
    const float k = (r * r) / 8.0;

    const float nom   = NdotV;
    const float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float geometry_smith(float NdotL, float NdotV, float roughness)
{
    return geometry_schlick_GGX(NdotL, roughness) * geometry_schlick_GGX(NdotV, roughness);
}

vec3 fresnel_schlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// Cook-Torrance BRDF
vec3 BRDF(vec3 V, vec3 N, vec3 L)
{
	const vec3  albedo    = texture(albedo_map, vert.tex_coord).rgb;
	const float metalness = texture(metallic_map, vert.tex_coord).r;
	const float roughness = texture(roughness_map, vert.tex_coord).r;
	const vec3  emissive  = texture(emissive_map, vert.tex_coord).rgb;

	const vec3 H = normalize(V + L);

	const float NdotL = max(dot(N, L), 0.0);
	const float NdotV = max(dot(N, V), 0.0);
	const float NdotH = max(dot(N, H), 0.0);

	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metalness);

	const float NDF = distribution_GGX(N, H, roughness);
    const float G   = geometry_smith(NdotL, NdotV, roughness);
    const vec3  F   = fresnel_schlick(clamp(NdotV, 0.0, 1.0), F0);
           
    const vec3  numerator   = NDF * G * F; 
    const float denominator = 4.0 * NdotV * NdotL + 0.0001; // + 0.0001 防止零除
    const vec3  specular    = numerator / denominator;

	vec3 ks = F;
    vec3 kd = vec3(1.0) - ks;
    kd *= 1.0 - metalness;

	// TODO
	const vec3 light_pos   = vec3(0.0);
	const vec3 light_color = vec3(3.0);

	float distance    = length(light_pos - vert.position);
    float attenuation = 1.0 / (distance * distance);
    // vec3  radiance    = light_color * attenuation;
    vec3  radiance    = light_color;

	return (kd * albedo / PI + specular) * radiance * NdotL;
}

void main()
{
	mat4 inv_view = inverse(mat.view);
	vec3 cam_pos  = inv_view[3].xyz;
	vec3 cam_dir  = -normalize(inv_view[2].xyz);

	const vec3 normal = vert.normal;
	// const vec3 normal = get_normal();
	
	vec3 V = normalize(cam_pos - vert.position);
	vec3 L = normalize(vec3(-1.0, 0.0, -0.5));
	
	const vec3  albedo    = texture(albedo_map, vert.tex_coord).rgb;
	const float occlusion = texture(occlusion_map, vert.tex_coord).r;

	// vec3 ambient = vec3(0.3) * albedo * occlusion;
	vec3 ambient = vec3(0.3) * albedo * 1.0;

	frag_color = vec4(ambient + BRDF(V, normal, L), 1.0);
		
	// frag_color = vec4(texture(normal_map, vert.tex_coord).rgb, 1.0);

#if DEBUG_ONLY_NORMAL
	frag_color = vec4(normal, 1.0);              // DEBUG: normal only
#endif
	// frag_color = vec4(emissive, 1.0);            // DEBUG: emissive only
	// frag_color = vec4(vec3(metalness), 1.0);     // DEBUG: metalness only
	// frag_color = vec4(vec3(roughness), 1.0);     // DEBUG: roughness only
#if DEBUG_ONLY_OCCLUSION
	frag_color = vec4(vec3(occlusion), 1.0);     // DEBUG: occlusion only
#endif
	// frag_color = vec4(vert.tex_coord, 0.0, 1.0); // DEBUG: uv only
}
