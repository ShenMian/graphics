// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKCommandQueue.h"
#include "../CommandQueue.h"
#include "VKCommandBuffer.h"
#include <numeric>

#include "VKRenderer.h"

VKCommandQueue::VKCommandQueue()
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	handle_        = renderer->get_device().get_queue(VKDevice::QueueType::Graphics);
}

void VKCommandQueue::submit(std::shared_ptr<CommandBuffer> command_buffer)
{
	auto  renderer  = reinterpret_cast<VKRenderer*>(Renderer::get());
	auto& device    = renderer->get_device();
	auto& swapchain = renderer->get_swapchain();

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, renderer->get_swapchain(), std::numeric_limits<uint64_t>::max(),
	                      swapchain.get_image_available_semaphores()[0], VK_NULL_HANDLE, &imageIndex);

	const auto vkCommandBuffer = std::dynamic_pointer_cast<VKCommandBuffer>(command_buffer);

	VkSemaphore          waitSemaphores[]   = {swapchain.get_image_available_semaphores()[0]};
	VkSemaphore          signalSemaphores[] = {swapchain.get_render_finished_semaphores()[0]};
	VkPipelineStageFlags waitStages[]       = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	VkCommandBuffer      commandBuffers[]   = {*vkCommandBuffer};

	VkSubmitInfo submitInfo         = {};
	submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount   = 1;
	submitInfo.pWaitSemaphores      = waitSemaphores;
	submitInfo.pWaitDstStageMask    = waitStages;
	submitInfo.commandBufferCount   = 1;
	submitInfo.pCommandBuffers      = commandBuffers;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores    = signalSemaphores;

	if(vkQueueSubmit(handle_, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
		throw std::runtime_error("failed to submit command buffer");


	const auto presentQueue = device.get_queue(VKDevice::QueueType::Present);

	VkSwapchainKHR swapchains[] = {renderer->get_swapchain()};

	VkPresentInfoKHR presentInfo   = {};
	presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores    = signalSemaphores;
	presentInfo.swapchainCount     = 1;
	presentInfo.pSwapchains        = swapchains;
	presentInfo.pImageIndices      = &imageIndex;
	vkQueuePresentKHR(presentQueue, &presentInfo);

	vkQueueWaitIdle(presentQueue);
}
