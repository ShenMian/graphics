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
	uint32_t           id_;
	std::atomic<State> state_ = State::Unloaded;

	inline static std::set<uint32_t> used_ids_;

	inline static std::uniform_int_distribution<uint32_t> distrib_;
	inline static std::mt19937                            generator_ = std::mt19937(std::random_device()());
};

inline RID::RID()
{
	assert(used_ids_.size() <= std::numeric_limits<uint32_t>::max());
#if 1
	do
	{
		id_ = distrib_(generator_);
	} while(used_ids_.contains(id_));
#else
	id = distrib(generator);
#endif
}

inline RID::operator uint32_t() const
{
	return id_;
}
