// Copyright 2022 ShenMian
// License(Apache-2.0)

#version 450

struct Vert
{
    vec3 position;
    vec3 normal;
    vec2 tex_coord;
    vec3 tangent;
    vec3 bitangent;
};

struct Bone
{
    uvec4 id;
    vec4  weight;
};

layout(binding = 0) uniform Matrices
{
    mat4 view;
    mat4 proj;
    mat4 model;
} mat;

/*
const uint max_bones = 100;
layout(binding = 1) uniform Animation
{
    mat4 matrices[max_bones];
    Bone bones[max_bones];
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
    /*
    Bone bone = anim.bones[gl_VertexIndex];
    mat4 offset = anim.matrices[bone.id[0]]
        + anim.matrices[bone.id[1]]
        + anim.matrices[bone.id[2]]
        + anim.matrices[bone.id[3]];
    */

    mat4 inv_model = inverse(mat.model);
    // mat4 inv_model = mat.model;

    vert.position  = vec3(inv_model * vec4(position, 1.0));
    vert.normal    = normalize(mat3(transpose(inverse(mat.model))) * normal);
    vert.tex_coord = tex_coord;
    vert.tangent   = tangent;
    vert.bitangent = bitangent;

    // vert.position.x = -vert.position.x;
    // vert.position.z = -vert.position.z;

    gl_Position = mat.proj * mat.view * vec4(vert.position, 1.0);
}
