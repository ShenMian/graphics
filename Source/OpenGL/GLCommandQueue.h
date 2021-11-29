// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../CommandQueue.h"

enum class GLOpcode;

class GLCommandQueue : public CommandQueue
{
public:
	void submit(std::shared_ptr<CommandBuffer> commandBuffer);

private:
	size_t execute(GLOpcode opcode, const uint8_t* pc);
};
