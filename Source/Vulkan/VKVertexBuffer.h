// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VertexBuffer.h"
#include "VKBuffer.h"
#include <vulkan/vulkan.h>

class VKVertexBuffer : public VertexBuffer
{
public:
	VKVertexBuffer(const void* data, size_t size, const VertexLayout& layout, Usage usage);
	virtual ~VKVertexBuffer();

	void map() override;
	void unmap() override;

	void write(const void* data, size_t size) override;

	VkPipelineVertexInputStateCreateInfo getInfo();

	void bind() override;

private:
	VKBuffer handle;

	std::vector<VkVertexInputBindingDescription>   bindings;
	std::vector<VkVertexInputAttributeDescription> attribs;
};
