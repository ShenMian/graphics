// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "PipelineLayout.h"
#include "VertexFormat.h"
#include "Viewport.hpp"
#include <memory>

class Program;
class VertexBuffer;

/**
 * @brief 多边形绘制模式
 */
enum class PolygonMode
{
	Fill,     ///< 填充的.
	Wireframe ///< 线框.
};

/**
 * @brief 面剔除.
 */
enum class CullMode
{
	Disabled, ///< 禁用.
	Front,    ///< 正面剔除.
	Back,     ///< 背面剔除.
};

enum class CompareOp
{
	AlwaysPass,
	NeverPass,
	Less,
	Equal,
	LessEqual,
	Greater,
	NotEqual,
	GreaterEqual,
};

struct RasterizerDescriptor
{
	PolygonMode polygonMode = PolygonMode::Fill;
	CullMode    cullMode    = CullMode::Disabled;
	float       lineWidth   = 1.0f;
};

struct DepthDescriptor
{
	bool      enableTest  = true;
	bool      enableWrite = true;
	CompareOp compareOp   = CompareOp::Less;
};

/**
 * @brief 图形管线.
 */
class Pipeline
{
public:
	struct Descriptor
	{
		PipelineLayout           layout;
		VertexFormat             vertexFormat;
		std::shared_ptr<Program> program;
		std::vector<Viewport>    viewports;

		RasterizerDescriptor rasterizer;
		DepthDescriptor      depth;
	};

	[[nodiscard]] static std::shared_ptr<Pipeline> create(const Descriptor& desc);

	virtual void bind() = 0;

protected:
	Pipeline(const Descriptor& desc);
	virtual ~Pipeline() = default;

	Descriptor desc;
};
