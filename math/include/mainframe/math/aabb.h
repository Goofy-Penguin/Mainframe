#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace mainframe {
	namespace math {
		template <class NumberType>
		class AABB_t {
		private:

		public:
			NumberType x = 0, y = 0, w = 0, h = 0;

			AABB_t() = default;
			~AABB_t() = default;

			AABB_t(NumberType _x, NumberType _y = 0, NumberType _w = 0, NumberType _h = 0) : x(_x), y(_y), w(_w), h(_h) {}
		};

		using AABBd = AABB_t<double>;
		using AABBf = AABB_t<float>;
		using AABBi = AABB_t<int>;
		using AABB = AABBf;
	}
}