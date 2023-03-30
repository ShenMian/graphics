// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "CommandQueue.h"
#include <vulkan/vulkan.h>

class VKCommandQueue : public CommandQueue
{
public:
	VKCommandQueue();

	void submit(std::shared_ptr<CommandBuffer> command_buffer) override;

private:
	VkQueue handle_;
};
