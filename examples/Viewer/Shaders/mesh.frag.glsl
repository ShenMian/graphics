#version 450
#extension GL_KHR_vulkan_glsl : enable
precision highp float;

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

layout(location = 0) uniform sampler2D diffuse_map;
layout(location = 1) uniform sampler2D specular_map;
layout(location = 2) uniform sampler2D ambient_map;
layout(location = 3) uniform sampler2D emissive_map;
layout(location = 4) uniform sampler2D height_map;
layout(location = 5) uniform sampler2D normal_map;
layout(location = 6) uniform sampler2D shininess_map;
layout(location = 7) uniform sampler2D opacity_map;

layout(location = 0) in Vert vert;

layout(location = 0) out vec4 frag_color;

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

vec3 calc_dir_light(DirectionalLight light, vec3 N, vec3 V)
{
	const vec3 L = light.direction;

	// 环境光
	const float ka      = 0.3;
    const vec3  ambient = ka * light.color;

	// 漫反射
	const float kd             = 1.0;
	const float diffuse_amount = max(dot(N, L), 0.0);
	const vec3  diffuse        = kd * diffuse_amount * light.color;

	// 镜面反射
	const float ks              = 0.5;
	const float shininess       = 4.0;
	const vec3  R               = reflect(-L, N);
	const float specular_amount = pow(max(dot(R, V), 0.0), shininess);
	const vec3  specular        = ks * specular_amount * light.color;

	vec3 ambient_color  = texture(diffuse_map, vert.tex_coord).rgb;
	vec3 diffuse_color  = texture(diffuse_map, vert.tex_coord).rgb;
	vec3 specular_color = texture(specular_map, vert.tex_coord).rgb;

	return (ambient * ambient_color + diffuse * diffuse_color + specular * specular_color) * light.intesity;
}

vec3 calc_light(vec3 N, vec3 V)
{
	DirectionalLight light;
	light.color     = vec3(1.0);
	light.intesity  = 1.0;
	light.direction = vec3(-1.0, 0.5, -0.5);

	vec3 lighting = vec3(0.0);
	lighting += calc_dir_light(light, N, V);
	return lighting;
}

void main()
{
	mat4 inv_view = inverse(mat.view);
	vec3 cam_pos  = inv_view[3].xyz;
	vec3 cam_dir  = -normalize(inv_view[2].xyz);

	vec4  albedo   = texture(diffuse_map, vert.tex_coord).rgba;
	vec3  emissive = texture(emissive_map, vert.tex_coord).rgb;
	vec3  normal   = vert.normal;
	// vec3  normal   = get_normal();

	if(albedo.a < 0.5)
		discard;

	frag_color = albedo;

	if(vec3(albedo) == vec3(0.0))
		frag_color = vec4(vert.tex_coord, 0.0, 1.0);
		
	vec3 lighting = calc_light(normal, cam_dir);
	frag_color = vec4(lighting, 1.0);
	
	// frag_color = vec4(normal, 1.0);                     // DEBUG: normal only
	// frag_color = texture(specular_map, vert.tex_coord); // DEBUG: specular only
	// frag_color = vec4(vert.tex_coord, 0.0, 1.0);        // DEBUG: uv only
	// frag_color = vec4(emissive, 1.0);                   // DEBUG: emissive only
}
