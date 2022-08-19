// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <chrono>

/** @addtogroup core
 *  @{
 */

/**
 * @brief 计时器.
 */
class Clock
{
public:
	using clock = std::chrono::high_resolution_clock;

	Clock()
	{
		restart();
	}

	void restart()
	{
		start = clock::now();
	}

	auto getMilliseconds() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start).count();
	}

	auto getSeconds() const
	{
		return static_cast<double>(getMilliseconds()) / 1000.0;
	}

private:
	std::chrono::time_point<clock> start;
};

/** @}*/
