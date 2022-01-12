// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "PipelineLayout.h"
#include "Viewport.hpp"
#include <memory>

class Program;
class VertexBuffer;

/**
 * @brief 图形管线.
 */
class Pipeline
{
public:
	struct Descriptor;

	static std::shared_ptr<Pipeline> create(const Descriptor& desc);

	Pipeline(const Descriptor& desc);

	std::shared_ptr<Program> program;
};

/**
 * @brief 多边形绘制模式
 */
enum class PolygonMode
{
	Fill,      ///< 填充的.
	Wireframe  ///< 线框.
};

/**
 * @brief 面剔除.
 */
enum class CullMode
{
	Disabled,   ///< 禁用.
	Front,      ///< 正面剔除.
	Back,       ///< 背面剔除.
};

struct RasterizerDescriptor
{
    PolygonMode polygonMode = PolygonMode::Fill;
    CullMode    cullMode    = CullMode::Disabled;
};

struct Pipeline::Descriptor
{
	RasterizerDescriptor rasterizer;

	PipelineLayout                layout;
	std::shared_ptr<Program>      program;
	std::vector<Viewport>         viewports;

	std::shared_ptr<VertexBuffer> vertexBuffer;
};
