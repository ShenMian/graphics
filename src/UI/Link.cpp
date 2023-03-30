// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Link.h"
#include "Pin.h"

namespace ui
{

Link::Link(const Pin& start, const Pin& end) : start_(start), end_(end)
{
}

const Pin& Link::get_start() const noexcept
{
	return start_;
}

const Pin& Link::get_end() const noexcept
{
	return end_;
}

}; // namespace ui
