// Copyright 2022 ShenMian
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
	renderer_     = reinterpret_cast<VKRenderer*>(Renderer::get());
	auto& device = renderer_->get_device();

	VkBufferCreateInfo bufInfo = {};
	bufInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufInfo.size               = size;
	bufInfo.usage              = VKUsage[type];
	bufInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;
	if(vkCreateBuffer(device, &bufInfo, nullptr, &handle_) != VK_SUCCESS)
		throw std::runtime_error("failed to create buffer");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, handle_, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize       = memRequirements.size;
	allocInfo.memoryTypeIndex      = find_memory_type(memRequirements.memoryTypeBits, properties);
	if(vkAllocateMemory(device, &allocInfo, nullptr, &memory_) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate buffer memory");

	vkBindBufferMemory(device, handle_, memory_, 0);
}

VKBuffer::~VKBuffer()
{
	auto& device = renderer_->get_device();
	vkDestroyBuffer(device, handle_, nullptr);
	vkFreeMemory(device, memory_, nullptr);
}

void VKBuffer::map(size_t size, size_t offset)
{
	auto& device = renderer_->get_device();
	if(vkMapMemory(device, memory_, offset, size, 0, &data_) != VK_SUCCESS)
		throw std::runtime_error("failed to map buffer memory");
}

void VKBuffer::unmap()
{
	auto& device = renderer_->get_device();
	if(data_ == nullptr)
		return;
	vkUnmapMemory(device, memory_);
	data_ = nullptr;
}

void VKBuffer::flush(size_t size, size_t offset)
{
	auto&               device = renderer_->get_device();
	VkMappedMemoryRange info;
	info.memory = memory_;
	info.offset = offset;
	info.size   = size;
	vkFlushMappedMemoryRanges(device, 1, &info);
}

VKBuffer::operator VkBuffer() noexcept
{
	return handle_;
}

VKBuffer::operator VkBuffer() const noexcept
{
	return handle_;
}

uint32_t VKBuffer::find_memory_type(uint32_t type, VkMemoryPropertyFlags properties) const
{
	auto&                            physicalDevice = renderer_->get_physical_device();
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		if((type & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	throw std::runtime_error("failed to find suitable memory type");
}
