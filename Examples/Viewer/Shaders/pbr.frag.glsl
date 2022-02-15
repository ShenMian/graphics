// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

const float PI = 3.141592654;

layout (location = 0) out vec4 frag_color;

struct Material
{
    sampler2D albedo;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D ao;
    sampler2D emissive;
    sampler2D normal;
};

in Out
{
    vec3 position;
    vec3 normal;
    vec2 tex_coord;
    vec3 tangent;
    vec3 bitangent;
    mat3 TBN;
} vert;

uniform mat4 view;
uniform Material mat;

void main()
{
    mat4 invView       = inverse(view);
    vec3 cam_position  = invView[3].xyz;
    vec3 cam_direction = -normalize(invView[2].xyz);

    vec3 color = vec3(vert.tex_coord, 0.0);

    if(dot(normalize(vert.position - cam_position), cam_direction) > 0.9)
        color.r = 1;

    frag_color = vec4(color, 1.0);

	vec3  albedo    = texture(mat.albedo,    vert.tex_coord).rgb;
	float metalness = texture(mat.metallic,  vert.tex_coord).r;
	float roughness = texture(mat.roughness, vert.tex_coord).r;
    float ao        = texture(mat.ao,        vert.tex_coord).r;
    vec3  emissive  = texture(mat.emissive,  vert.tex_coord).rgb;

    vec3 N = normalize(2.0 * texture(mat.normal, vert.tex_coord).rgb - 1.0);
    N = normalize(vert.TBN * vert.normal);

    vec3 direct_lighting = vec3(0.0);
}
