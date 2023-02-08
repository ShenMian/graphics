// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

enum class Format
{
	// int
	R8I,
	RG8I,
	RGB8I,
	RGBA8I,

	R16I,
	RG16I,
	RGB16I,
	RGBA16I,

	R32I,
	RG32I,
	RGB32I,
	RGBA32I,

	// unsigned int
	R8U,
	RG8U,
	RGB8U,
	RGBA8U,

	R16U,
	RG16U,
	RGB16U,
	RGBA16U,

	R32U,
	RG32U,
	RGB32U,
	RGBA32U,

	// float
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

	// compressed
	RGBA_DXT1,
	RGBA_DXT3,
	RGBA_DXT5,

	Depth,
	Stencil,
	DepthStencil,

	Unknown
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
