// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <cassert>
#include <set>
#include <random>

class RID
{
public:
	RID();

	// RID(const RID&) = delete;

	operator uint32_t() const;

private:
	uint32_t id;

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
