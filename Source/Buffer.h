// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

class Buffer
{
public:
	enum class Type
	{
		Vertex,
		Index,
		Uniform
	};

	enum class Usage
	{
		Static,
		Dynamic,
		Stream
	};

	Buffer(size_t size, Type type, Usage usage);

	void* getData();

	size_t getSize() const;
	Type getType() const;
	Usage getUsage() const;

	virtual void map(size_t size = -1, size_t offset = 0) = 0;
	virtual void unmap() = 0;

protected:
	size_t size;
	Type   type;
	Usage  usage;
	void*  data = nullptr;
};
