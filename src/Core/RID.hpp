// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <atomic>
#include <cassert>
#include <random>
#include <set>

class RID
{
public:
	enum class State
	{
		Unloaded,
		Loading,
		Loaded
	};

	RID();

	// RID(const RID&) = delete;

	operator uint32_t() const;

private:
	uint32_t           id;
	std::atomic<State> state = State::Unloaded;

	inline static std::set<uint32_t> usedIds;

	inline static std::uniform_int_distribution<uint32_t> distrib;
	inline static std::mt19937                            generator = std::mt19937(std::random_device()());
};

inline RID::RID()
{
	assert(usedIds.size() <= std::numeric_limits<uint32_t>::max());
#if 1
	do
	{
		id = distrib(generator);
	} while(usedIds.contains(id));
#else
	id = distrib(generator);
#endif
}

inline RID::operator uint32_t() const
{
	return id;
}
