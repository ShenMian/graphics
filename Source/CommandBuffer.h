// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include "Program.h"
#include <Math/Vector4.hpp>
#include <memory>

struct ClearFlag
{
	enum
	{
		Color,
		Depth,
		Stencil
	};
};

class CommandBuffer
{
public:
	static std::shared_ptr<CommandBuffer> create();

	virtual void begin() = 0;

	virtual void end() = 0;

	virtual void execute() = 0;

	virtual void setViewport(const Vector2i& origin, const Vector2i& size) = 0;

	virtual void clear(uint8_t flags) = 0;
	virtual void setClearColor(const Vector4& color) = 0;
	virtual void setClearDepth(float depth) = 0;
	virtual void setClearStencil() = 0;

private:
	std::shared_ptr<Program> program;
};