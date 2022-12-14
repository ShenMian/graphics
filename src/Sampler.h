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

	Filter getMinFilter() const noexcept;
	Filter getMagFilter() const noexcept;

	Warp getSWarpMode() const noexcept;
	Warp getTWarpMode() const noexcept;
	Warp getRWarpMode() const noexcept;

protected:
	Filter minFilter;
	Filter magFilter;
	Warp   SWarp;
	Warp   TWarp;
	Warp   RWarp;
};
