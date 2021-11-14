// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "CommandBuffer.h"
#include <memory>

/**
 * @brief 命令队列.
 */
class CommandQueue
{
public:
	static std::shared_ptr<CommandQueue> create();

	virtual void submit(std::shared_ptr<CommandBuffer> commandBuffer) = 0;
};