// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Sampler.h"
#include <cassert>

Sampler::Sampler(const Descriptor& desc)
    : minFilter(desc.minFilter), magFilter(desc.magFilter), SWarp(desc.SWarp), TWarp(desc.TWarp), RWarp(desc.RWarp)
{
	assert(minFilter == Filter::Nearest || minFilter == Filter::Bilinear || minFilter == Filter::Trilinear);
	assert(magFilter == Filter::Nearest || magFilter == Filter::Bilinear);
}

Sampler::Filter Sampler::getMinFilter() const noexcept
{
	return minFilter;
}

Sampler::Filter Sampler::getMagFilter() const noexcept
{
	return magFilter;
}

Sampler::Warp Sampler::getSWarpMode() const noexcept
{
	return SWarp;
}

Sampler::Warp Sampler::getTWarpMode() const noexcept
{
	return TWarp;
}

Sampler::Warp Sampler::getRWarpMode() const noexcept
{
	return RWarp;
}
