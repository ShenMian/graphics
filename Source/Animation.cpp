// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Animation.h"

Animation::Animation(const std::string& name, float duration, uint32_t tickRate)
	: name(name), duration(duration), tickRate(tickRate)
{
}

const std::string& Animation::getName() const
{
	return name;
}

float Animation::getDuration() const
{
	return duration;
}

uint32_t Animation::getTickRate() const
{
	return tickRate;
}
