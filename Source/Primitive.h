// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

class Mesh;

class Primitive
{
public:
	/**
	 * @brief 生成平面.
	 */
	static Mesh makePlane(unsigned int x, unsigned int z);

	/**
	 * @brief 生成立方体.
	 */
	// static Mesh makeCube(unsigned int x, unsigned int y);

	/**
	 * @brief 生成包含UV坐标的球.
	 */
	static Mesh makeSphere(unsigned int lat, unsigned int lon);

	/**
	 * @brief 生成胶囊体.
	 *
	 * @param resolution 分辨率. 越高顶点数越多, 越接近胶囊体.
	 * @param height     高度.
	 * @param radius     半径.
	 */
	static Mesh makeCapsule(unsigned int resolution, float height, float radius);

	/**
	 * @brief 生成 ico 球.
	 */
	static Mesh makeIcoSphere(unsigned int resolution);
};
