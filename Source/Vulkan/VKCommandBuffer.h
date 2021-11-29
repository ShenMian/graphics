// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../CommandBuffer.h"

class VKCommandBuffer : public CommandBuffer
{
public:
	void begin() override;
	void end() override;

	void setViewport(const Vector2i& origin, const Vector2i& size) override;

	void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
	void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;

	void clear(uint8_t flags) override;
	void setClearColor(const Vector4& color) override;
	void setClearDepth(float depth) override;
	void setClearStencil() override;

	void draw(size_t first, size_t num) override;
};
