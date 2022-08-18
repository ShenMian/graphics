// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKBuffer.h"
#include <unordered_map>

namespace
{

std::unordered_map<Buffer::Type, VkBufferUsageFlags> VKUsage = {
    {Buffer::Type::Vertex, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT},
    {Buffer::Type::Index, VK_BUFFER_USAGE_INDEX_BUFFER_BIT},
    {Buffer::Type::Uniform, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT}};

}

VKBuffer::VKBuffer(size_t size, Buffer::Type type, Buffer::Usage usage, VkMemoryPropertyFlags properties)
    : Buffer(size, type, usage)
{
	renderer     = reinterpret_cast<VKRenderer*>(Renderer::get());
	auto& device = renderer->getDevice();

	VkBufferCreateInfo bufInfo = {};
	bufInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufInfo.size               = size;
	bufInfo.usage              = VKUsage[type];
	bufInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;
	if(vkCreateBuffer(device, &bufInfo, nullptr, &handle) != VK_SUCCESS)
		throw std::runtime_error("failed to create buffer");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, handle, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize       = memRequirements.size;
	allocInfo.memoryTypeIndex      = findMemoryType(memRequirements.memoryTypeBits, properties);
	if(vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate buffer memory");

	vkBindBufferMemory(device, handle, memory, 0);
}

VKBuffer::~VKBuffer()
{
	auto& device = renderer->getDevice();
	vkDestroyBuffer(device, handle, nullptr);
	vkFreeMemory(device, memory, nullptr);
}

void VKBuffer::map(size_t size, size_t offset)
{
	auto& device = renderer->getDevice();
	if(vkMapMemory(device, memory, offset, size, 0, &data) != VK_SUCCESS)
		throw std::runtime_error("failed to map buffer memory");
}

void VKBuffer::unmap()
{
	auto& device = renderer->getDevice();
	if(data == nullptr)
		return;
	vkUnmapMemory(device, memory);
	data = nullptr;
}

void VKBuffer::flush(size_t size, size_t offset)
{
	auto&               device = renderer->getDevice();
	VkMappedMemoryRange info;
	info.memory = memory;
	info.offset = offset;
	info.size   = size;
	vkFlushMappedMemoryRanges(device, 1, &info);
}

VKBuffer::operator VkBuffer()
{
	return handle;
}

VKBuffer::operator VkBuffer() const
{
	return handle;
}

uint32_t VKBuffer::findMemoryType(uint32_t type, VkMemoryPropertyFlags properties)
{
	auto&                            physicalDevice = renderer->getPhysicalDevice();
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		if((type & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	throw std::runtime_error("failed to find suitable memory type");
}
