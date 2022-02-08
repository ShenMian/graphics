// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../CommandBuffer.h"
#include <vector>
#include <vulkan/vulkan.h>

class VKCommandBuffer : public CommandBuffer
{
public:
	VKCommandBuffer();
	virtual ~VKCommandBuffer();

	void begin() override;
	void end() override;

	void beginRenderPass(std::shared_ptr<Pipeline> pipeline) override;
	void endRenderPass() override;

	void setViewport(const Viewport& viewport) override;

	void setPipeline(std::shared_ptr<Pipeline> pipeline) override;

	void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
	void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;

	void clear(uint8_t flags) override;
	void setClearColor(const Vector4& color) override;
	void setClearDepth(float depth) override;
	void setClearStencil() override;

	void draw(uint32_t vertexCount, uint32_t firstVertex) override;
	void drawIndexed(uint32_t indexCount, uint32_t firstIndex) override;

	operator VkCommandBuffer() const;

private:
	std::vector<VkCommandBuffer> handles;
};
