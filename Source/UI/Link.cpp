// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Link.h"
#include "Pin.h"

namespace ui
{

Link::Link(const Pin& start, const Pin& end) : start(start), end(end)
{
}

const Pin& Link::getStart() const noexcept
{
	return start;
}

const Pin& Link::getEnd() const noexcept
{
	return end;
}

}; // namespace ui
