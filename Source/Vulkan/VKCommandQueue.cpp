// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKCommandQueue.h"
#include "VKCommandBuffer.h"
#include "../CommandQueue.h"

#include "VKRenderer.h"

VKCommandQueue::VKCommandQueue()
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	handle = renderer->getDevice().getQueue(VKDevice::QueueType::Graphics);
}

void VKCommandQueue::submit(std::shared_ptr<CommandBuffer> commandBuffer)
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	auto& device = renderer->getDevice();

	uint32_t imageIndex;
	vkAcquireNextImageKHR(renderer->getDevice(), renderer->getSwapchain(),
		UINT64_MAX, renderer->imageAvailableSemaphore, nullptr, &imageIndex);

	const auto vkCommandBuffer = std::dynamic_pointer_cast<VKCommandBuffer>(commandBuffer);

	VkSemaphore waitSemaphores[] = {renderer->imageAvailableSemaphore};
	VkSemaphore signalSemaphores[] = {renderer->renderFinishedSemaphore};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	VkCommandBuffer commandBuffers[] = {*vkCommandBuffer};

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = commandBuffers;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	
	if(vkQueueSubmit(handle, 1, &submitInfo, nullptr) != VK_SUCCESS)
		throw std::runtime_error("failed to submit command buffer");


	auto presentQueue = device.getQueue(VKDevice::QueueType::Present);

	VkSwapchainKHR swapchains[] = {renderer->getSwapchain()};

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapchains;
	presentInfo.pImageIndices = &imageIndex;
	vkQueuePresentKHR(presentQueue, &presentInfo);
}
