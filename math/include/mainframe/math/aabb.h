#pragma once

#include <mainframe/math/vector2.h>
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
			AABB_t(const Vector2_t<NumberType>& pos, const Vector2_t<NumberType>& size) : x(pos.x), y(pos.y), w(size.x), h(size.y) {}

			AABB_t<NumberType> clamp(const AABB_t<NumberType>& boundries) {
				AABB_t<NumberType> r = *this;

				if (r.x < boundries.x) {
					r.w += boundries.x - r.x;
					r.x = boundries.x;
				}

				if (r.y < boundries.y) {
					r.h += boundries.y - r.y;
					r.y = boundries.y;
				}

				float wdiff = (r.x + r.w) - (boundries.x + boundries.w);
				if (wdiff > 0) r.w -= wdiff;

				float hdiff = (r.y + r.h) - (boundries.y + boundries.h);
				if (hdiff > 0) r.h -= hdiff;

				return r;
			}
		};

		using AABBd = AABB_t<double>;
		using AABBf = AABB_t<float>;
		using AABBi = AABB_t<int>;
		using AABB = AABBf;
	}
}