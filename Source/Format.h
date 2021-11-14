// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

enum class Format
{
    R8F,
    RG8F,
    RGB8F,
    RGBA8F,

    R16F,
    RG16F,
    RGB16F,
    RGBA16F,

    R32F,
    RG32F,
    RGB32F,
    RGBA32F,

    Depth,
    Stencil,
    DepthStencil
};

enum class DataType
{
    Int8,
    Int16,
    Int32,

    Uint8,
    Uint16,
    Uint32,

    Float16,
    Float32,
    Float64
};