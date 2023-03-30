// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Sampler.h"
#include <cassert>

Sampler::Sampler(const Descriptor& desc)
    : min_filter_(desc.minFilter), mag_filter_(desc.magFilter), s_warp_(desc.SWarp), t_warp_(desc.TWarp), r_warp_(desc.RWarp)
{
	assert(min_filter_ == Filter::Nearest || min_filter_ == Filter::Bilinear || min_filter_ == Filter::Trilinear);
	assert(mag_filter_ == Filter::Nearest || mag_filter_ == Filter::Bilinear);
}

Sampler::Filter Sampler::get_min_filter() const noexcept
{
	return min_filter_;
}

Sampler::Filter Sampler::get_mag_filter() const noexcept
{
	return mag_filter_;
}

Sampler::Warp Sampler::get_s_warp_mode() const noexcept
{
	return s_warp_;
}

Sampler::Warp Sampler::get_t_warp_mode() const noexcept
{
	return t_warp_;
}

Sampler::Warp Sampler::get_r_warp_mode() const noexcept
{
	return r_warp_;
}
