// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKSampler.h"
#include <unordered_map>

namespace
{

std::unordered_map<Sampler::Filter, VkFilter> VKFilter = {{Sampler::Filter::Nearest, VK_FILTER_NEAREST},
                                                          {Sampler::Filter::Bilinear, VK_FILTER_LINEAR},
                                                          {Sampler::Filter::Trilinear, VK_FILTER_LINEAR /* TODO */}};

std::unordered_map<Sampler::Warp, VkSamplerAddressMode> VKWarp = {
    {Sampler::Warp::Repeat, VK_SAMPLER_ADDRESS_MODE_REPEAT},
    {Sampler::Warp::MirrorRepeat, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT},
    {Sampler::Warp::ClampToEdge, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE}};

} // namespace

VKSampler::VKSampler(const Descriptor& desc) : Sampler(desc)
{
	VkSamplerCreateInfo info = {};
	info.sType               = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	info.minFilter           = VKFilter[min_filter_];
	info.magFilter           = VKFilter[mag_filter_];
	info.addressModeU        = VKWarp[s_warp_];
	info.addressModeV        = VKWarp[t_warp_];
	info.addressModeW        = VKWarp[r_warp_];
	// vkCreateSampler()
}

VKSampler::~VKSampler()
{
	// vkDestroySampler()
}
