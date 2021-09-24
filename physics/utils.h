#pragma once
#include "olcPixelGameEngine.h"

namespace utils {

	template<typename T>
	inline T distance(const olc::v2d_generic<T>& p1, const olc::v2d_generic<T>& p2) noexcept
	{
		return std::hypot(p1.x - p2.x, p1.y - p2.y);
	}

}