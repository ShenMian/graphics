// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Animation.h"

Animation::Animation(const std::string& name, float duration, uint32_t tick_rate)
    : name_(name), duration_(duration), tick_rate_(tick_rate)
{
}

const std::string& Animation::get_name() const
{
	return name_;
}

float Animation::get_duration() const
{
	return duration_;
}

uint32_t Animation::get_tick_rate() const
{
	return tick_rate_;
}
