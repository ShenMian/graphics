// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include "CommandBuffer.h"
#include <memory>

class CommandQueue
{
public:
	static std::shared_ptr<CommandQueue> create();

	virtual void submit(std::shared_ptr<CommandBuffer> commandBuffer) = 0;
};