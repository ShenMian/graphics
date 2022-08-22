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
    float intensity;

    vec3 direction;
};

struct PointLight
{
    vec3  color;
    float intensity;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3  color;
    float intensity;

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

layout(binding = 1, std140) uniform Lights
{
	DirectionalLight dir_lights[1];
	int              dir_lights_size;
} lights;

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

#define DEBUG_ONLY_NORMAL 0
#define DEBUG_ONLY_UV     0

#define ENABLE_BLINN_PHONG  1
#define ENABLE_TOON_SHADING 0

const int   toon_color_levels = 4;
const float toon_scale_factor = 1.0 / toon_color_levels;
const float rim_light_power   = 2.0;

/**
  * @brief 获取轮廓光系数.
  *
  * @param N 表面法向量.
  * @param V 表面指向相机方向.
  */
float calc_rim_light_factor(vec3 V, vec3 N)
{
    float factor = dot(V, N);
    factor       = max(1.0 - factor, 0.0);
    factor       = pow(factor, rim_light_power);
    return factor;
}

/**
  * @brief 获取 TBN 矩阵.
  */
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

/**
  * @brief 获取向量.
  */
vec3 get_normal()
{
	vec3 tangent_normal = normalize(texture(normal_map, vert.tex_coord).rgb * 2.0 - 1.0);
	mat3 TBN            = get_tbn_matrix();

	return TBN * tangent_normal;
}

/**
  * @brief 计算平行光光照.
  *
  * @param light 平行光.
  * @param N     表面法向量.
  * @param V     表面指向相机方向.
  */
vec3 calc_dir_light(DirectionalLight light, vec3 N, vec3 V)
{
	const vec3 L = -light.direction;
	
	const float ka        = 0.3;  // 环境光系数
	const float kd        = 1.0;  // 漫反射系数
	const float ks        = 1.0;  // 镜面反射系数
	const float shininess = 32.0; // 镜面反射指数

	vec3 diffuse_color  = texture(diffuse_map, vert.tex_coord).rgb;
	vec3 specular_color = texture(specular_map, vert.tex_coord).rgb;

	if(diffuse_color == vec3(0.0))
		diffuse_color = vec3(0.5);

#if ENABLE_TOON_SHADING
	// 环境光
    const vec3 ambient = ka * light.color;

	// 漫反射
	const float diff    = ceil(max(dot(N, L), 0.0) * toon_color_levels) * toon_scale_factor;
	const vec3  diffuse = kd * diff * light.color;

	// 边缘光
	const float rim = calc_rim_light_factor(V, N);

	return ambient * diffuse_color + ((diffuse * diffuse_color + rim * diffuse_color) * light.intensity);
#else
	// 环境光
    const vec3 ambient = ka * light.color;

	// 漫反射
	const float diff    = max(dot(N, L), 0.0);
	const vec3  diffuse = kd * diff * light.color;

	// 镜面反射
	vec3 specular = vec3(0.0);
	if(diff > 0.0)
	{
#if ENABLE_BLINN_PHONG
		// Blinn-Phong
		const vec3  H    = normalize(L + V);
		const float spec = pow(max(dot(N, H), 0.0), shininess);
#else
		// Phong
		const vec3  R    = reflect(-L, N);
		const float spec = pow(max(dot(V, R), 0.0), shininess / 4.0);
#endif
		specular = ks * spec * light.color;
	}

	return ambient * diffuse_color + ((diffuse * diffuse_color + specular * specular_color) * light.intensity);
#endif
}

/**
  * @brief 计算点光源光照.
  *
  * @param light 点光源.
  * @param N     表面法向量.
  * @param V     表面指向相机方向.
  */
vec3 calc_point_light(PointLight light, vec3 N, vec3 V)
{
	const vec3  L = light.position - vert.position;

	DirectionalLight dir_light;
	dir_light.color     = light.color;
	dir_light.intensity = light.intensity;
	dir_light.direction = L;

	// 衰减率
	const float dis         = length(L);
    const float attenuation = light.intensity / (light.constant + light.linear * dis + light.quadratic * (dis * dis));

	// TODO: 会导致存在环境光
	return calc_dir_light(dir_light, N, V) * attenuation;
}

/**
  * @brief 计算聚光灯光照.
  *
  * @param light 聚光灯.
  * @param N     表面法向量.
  * @param V     表面指向相机方向.
  */
vec3 calc_spot_light(SpotLight light, vec3 N, vec3 V)
{
	const vec3 L = light.position - vert.position;

	const float theta     = dot(L, normalize(-light.direction)); 
    const float epsilon   = light.cutOff - light.outerCutOff;
    const float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	PointLight point_light;
	point_light.color     = light.color;
	point_light.intensity = light.intensity * intensity;
    point_light.position  = light.position;
    point_light.constant  = light.constant;
    point_light.linear    = light.linear;
    point_light.quadratic = light.quadratic;
	
	// TODO: 会导致存在环境光
	return calc_point_light(point_light, N, V);
}

/**
  * @brief 计算光照.
  *
  * @param N 表面法向量.
  * @param V 表面指向相机方向.
  */
vec3 calc_light(vec3 N, vec3 V)
{
	DirectionalLight dir_light;
	dir_light.color     = vec3(1.0);
	dir_light.intensity = 1.0;
	dir_light.direction = vec3(-1.0, 0.0, -0.5);

	PointLight point_light;
	point_light.color     = vec3(1.0, 0.0, 0.0);
	point_light.intensity = 10.0;
	point_light.position  = vec3(0.0);
	point_light.constant  = 1.0;
	point_light.linear    = 0.09;
	point_light.quadratic = 0.032;

	SpotLight spot_light;
	spot_light.color       = vec3(0.0, 1.0, 0.0);
	spot_light.intensity   = 10.0;
	spot_light.position    = vec3(0.0);
	spot_light.direction   = -V;
    spot_light.cutOff      = radians(12.5);
    spot_light.outerCutOff = radians(17.5);
	spot_light.constant    = 1.0;
	spot_light.linear      = 0.09;
	spot_light.quadratic   = 0.032;

	vec3 lighting = vec3(0.0);
	lighting += calc_dir_light(dir_light, N, V);
	// lighting += calc_point_light(point_light, N, V);
	// lighting += calc_spot_light(spot_light, N, V);

	// lighting += calc_dir_light(lights.dir_lights[0], N, V);
	/*
	for(int i = 0; i < lights.dir_lights_size; i++)
		lighting += calc_dir_light(lights.dir_lights[i], N, V);
	*/
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

	vec3 V = normalize(cam_pos - vert.position);
	vec3 lighting = calc_light(normal, V);
	frag_color = vec4(lighting, albedo.a);

#if DEBUG_ONLY_NORMAL
	frag_color = vec4(normal, 1.0);                     // DEBUG: normal only
#endif
#if DEBUG_ONLY_UV
	frag_color = vec4(vert.tex_coord, 0.0, 1.0);        // DEBUG: uv only
#endif
	// frag_color = vec4(emissive, 1.0);                   // DEBUG: emissive only
	// frag_color = texture(specular_map, vert.tex_coord); // DEBUG: specular only
}
