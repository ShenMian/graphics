#version 450

// Copyright 2021 SMS
// License(Apache-2.0)

struct Vert
{
    vec3 position;
    vec3 normal;
    vec2 tex_coord;
    vec3 tangent;
    vec3 bitangent;
    mat3 TBN;
};

layout(set = 0, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 proj;
    mat4 model;
} mat;

/*
const uint max_joints = 100;
layout(set = 0, binding = 1) uniform Animation
{
    mat4 joints[max_joints];
} anim;
*/

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

layout(location = 0) out Vert vert;

// FIXME: 其他物体的 z 坐标和摄像机的 z 坐标相反

void main()
{
    mat4 invModel = inverse(mat.model);
    // mat4 invModel = mat.model;

    vert.position  = vec3(invModel * vec4(position, 1.0));
    vert.normal    = normalize(mat3(transpose(inverse(mat.model))) * normal);
    vert.tex_coord = tex_coord;
    vert.tangent   = tangent;
    vert.bitangent = bitangent;

    vec3 T   = normalize(vec3(mat.model * vec4(tangent,   0.0)));
    vec3 B   = normalize(vec3(mat.model * vec4(bitangent, 0.0)));
    vec3 N   = normalize(vec3(mat.model * vec4(normal,    0.0)));
    vert.TBN = transpose(mat3(T, B, N));

    // vert.position.x = -vert.position.x;
    // vert.position.z = -vert.position.z;

    gl_Position = mat.proj * mat.view * vec4(vert.position, 1.0);
}
