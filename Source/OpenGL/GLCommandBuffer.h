// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include "../CommandBuffer.h"
#include <cstdint>
#include <vector>

enum class GLOpcode;

class GLCommandBuffer : public CommandBuffer
{
public:
	void begin() override;

	void end() override;

	void execute() override;

	void setViewport(const Vector2i& origin, const Vector2i& size) override;

	void clear(uint8_t flags) override;
	void setClearColor(const Vector4& color) override;
	void setClearDepth(float depth) override;
	void setClearStencil() override;

private:
	size_t execute(GLOpcode opcode, uint8_t* pc);

	void addCommand(GLOpcode opcode);
	template <typename T> T* addCommand(GLOpcode opcode);

	std::vector<uint8_t> buffer;
};

template<typename T>
inline T* GLCommandBuffer::addCommand(GLOpcode opcode)
{
	buffer.push_back(static_cast<uint8_t>(opcode));
	auto offset = buffer.size();
	buffer.resize(offset + sizeof(T));
	return reinterpret_cast<T*>(&buffer[offset]);
}