// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

class Sampler
{
public:
	// 纹理过滤方式
	enum class Filter
	{
		Nearest,  ///< 最近点采样, 临近过滤.
		Bilinear, ///< 双线性过滤.
		Trilinear ///< 三线性过滤.
	};

	// 纹理环绕方式(纹理寻址方式)
	enum class Warp
	{
		Repeat,      ///< 重复.
		ClampToEdge, ///< 超出边界后重复边缘, 产生边缘被拉伸的效果.
		MirrorRepeat ///< 镜像重复, 正像/镜像交替.
	};

	struct Descriptor
	{
		Filter minFilter = Filter::Nearest;
		Filter magFilter = Filter::Bilinear;

		Warp SWarp = Warp::Repeat;
		Warp TWarp = Warp::Repeat;
		Warp RWarp = Warp::Repeat;

		float maxAnisotropy = 0.f;
	};

	Sampler(const Descriptor& desc);

	Filter get_min_filter() const noexcept;
	Filter get_mag_filter() const noexcept;

	Warp get_s_warp_mode() const noexcept;
	Warp get_t_warp_mode() const noexcept;
	Warp get_r_warp_mode() const noexcept;

protected:
	Filter min_filter_;
	Filter mag_filter_;
	Warp   s_warp_;
	Warp   t_warp_;
	Warp   r_warp_;
};
