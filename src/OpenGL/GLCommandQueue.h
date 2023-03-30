// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "CommandQueue.h"

enum class GLOpcode;

class GLCommandQueue : public CommandQueue
{
public:
	void submit(std::shared_ptr<CommandBuffer> command_buffer) override;

private:
	size_t execute(GLOpcode opcode, const uint8_t* pc);
};
