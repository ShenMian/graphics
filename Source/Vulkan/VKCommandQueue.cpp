// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKCommandQueue.h"
#include "../CommandQueue.h"

void VKCommandQueue::submit(std::shared_ptr<CommandBuffer> commandBuffer)
{
	// TODO
	VkCommandBuffer commandBuffers[] = {0};

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitDstStageMask = 0;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = commandBuffers;
	submitInfo.signalSemaphoreCount = 0;
	// if(vkQueueSubmit(handle, 1, &submitInfo, ) != VK_SUCCESS)
	// 	throw std::runtime_error("failed to submit command buffer");
}
