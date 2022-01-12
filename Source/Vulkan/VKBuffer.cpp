// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKBuffer.h"

VKBuffer::VKBuffer(size_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	auto& device = renderer->getDevice();

	VkBufferCreateInfo bufInfo = {};
	bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufInfo.size = size;
	bufInfo.usage = usage;
	bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	if(vkCreateBuffer(device, &bufInfo, nullptr, &buffer) != VK_SUCCESS)
		throw std::runtime_error("failed to create buffer");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
	if(vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate buffer memory");

	vkBindBufferMemory(device, buffer, memory, 0);
}

VKBuffer::~VKBuffer()
{
	auto& device = renderer->getDevice();
	vkDestroyBuffer(device, buffer, nullptr);
	vkFreeMemory(device, memory, nullptr);
}

VkResult VKBuffer::map(VkDeviceSize size, VkDeviceSize offset)
{
	auto& device = renderer->getDevice();
	return vkMapMemory(device, memory, offset, size, 0, &data);
}

void VKBuffer::unmap()
{
	auto& device = renderer->getDevice();
	if(data == nullptr)
		return;
	vkUnmapMemory(device, memory);
	data = nullptr;
}

void* VKBuffer::getData()
{
	return data;
}

uint32_t VKBuffer::findMemoryType(uint32_t type, VkMemoryPropertyFlags properties)
{
	auto& physicalDevice = renderer->getPhysicalDevice();
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		if((type & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	throw std::runtime_error("failed to find suitable memory type");
}
