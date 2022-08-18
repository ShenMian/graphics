// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <optional>

class Mesh;

/**
 * @brief 基本网格生成器.
 */
class Primitive
{
public:
	/**
	 * @brief 生成平面.
	 */
	static std::optional<Mesh> makePlane(unsigned int x, unsigned int z);

	/**
	 * @brief 生成立方体.
	 */
	// static Mesh makeCube(unsigned int x, unsigned int y);

	/**
	 * @brief 生成胶囊体.
	 *
	 * @param resolution 分辨率. 越高顶点数越多, 越接近胶囊体.
	 * @param height     高度.
	 * @param radius     半径.
	 */
	static std::optional<Mesh> makeCapsule(unsigned int resolution, float height, float radius);

	/**
	 * @brief 生成包含 UV 坐标的球.
	 */
	static std::optional<Mesh> makeSphere(unsigned int lat, unsigned int lon);

	/**
	 * @brief 生成 ICO 球.
	 */
	static Mesh makeIcoSphere(unsigned int resolution);
};
