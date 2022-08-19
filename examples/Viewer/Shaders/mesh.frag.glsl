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

layout(location = 0) uniform sampler2D albedo;
layout(location = 1) uniform sampler2D metallic;
layout(location = 2) uniform sampler2D roughness;
layout(location = 3) uniform sampler2D ao;
layout(location = 4) uniform sampler2D emissive;
layout(location = 5) uniform sampler2D normal;

layout(location = 0) in Vert vert;

layout(location = 0) out vec4 frag_color;

vec3 calc_dir_light(DirectionalLight light, vec3 N, vec3 V)
{
	const vec3 L = light.direction;

	// 环境光
	const float  ka      = 0.4;
    const vec3   ambient = ka * light.color;

	// 漫反射
	const float kd             = 1.0;
	const float diffuse_amount = max(dot(N, L), 0.0);
	const vec3  diffuse        = kd * diffuse_amount * light.color;

	// 镜面反射
	const float ks              = 0.3;
	const float shininess       = 4.0;
	const vec3  reflect_dir     = reflect(-L, N);
	const float specular_amount = pow(max(dot(reflect_dir, V), 0.0), shininess);
	const vec3  specular        = ks * specular_amount * light.color;

	return (ambient + diffuse + specular) * light.intesity;
}

vec3 calc_light(vec3 N, vec3 V)
{
	DirectionalLight light;
	light.color     = vec3(1.0);
	light.intesity  = 0.8;
	light.direction = vec3(-1.0, 0.0, -1.0);

	vec3 lighting = vec3(0.0);
	lighting += calc_dir_light(light, N, V);
	return lighting;
}

void main()
{
	mat4 inv_view = inverse(mat.view);
	vec3 cam_pos  = inv_view[3].xyz;
	vec3 cam_dir  = -normalize(inv_view[2].xyz);

	// if(dot(normalize(vert.position - cam_pos), cam_dir) > 0.9)
	//     color.r = 1;

	vec4  albedo    = texture(albedo, vert.tex_coord).rgba;
	float metalness = texture(metallic, vert.tex_coord).r;
	float roughness = texture(roughness, vert.tex_coord).r;
	float ao        = texture(ao, vert.tex_coord).r;
	vec3  emissive  = texture(emissive, vert.tex_coord).rgb;
	// vec3  normal    = texture(normal, vert.tex_coord).rgb;
	// normal          = normalize(2.0 * normal - 1.0);
	// normal          = normalize(vert.TBN * vert.normal);
	vec3 normal     = vert.normal;

	if(albedo.a < 0.5)
		discard;

	frag_color = albedo;

	if(vec3(albedo) == vec3(0.0))
		frag_color = vec4(vert.tex_coord, 0.0, 0.0);
		
	// frag_color = vec4(0.5); // DEBUG: Lighting
	vec3 lighting = calc_light(normal, cam_dir);
	frag_color = frag_color * vec4(lighting, 0.0);

	// frag_color = vec4(normal, 0.0);              // DEBUG: Normal
	// frag_color = vec4(vert.tex_coord, 0.0, 0.0); // DEBUG: UV
}
