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
			Vector2_t<NumberType> pos;
			Vector2_t<NumberType> size;

			AABB_t() = default;
			~AABB_t() = default;

			AABB_t(NumberType x_, NumberType y_ = 0, NumberType w_ = 0, NumberType h_ = 0) : pos(x_, y_), size(w_, h_) {}
			AABB_t(const Vector2_t<NumberType>& pos_, const Vector2_t<NumberType>& size_) : pos(pos_), size(size_) {}

			AABB_t<NumberType> clamp(const AABB_t<NumberType>& boundries) {
				AABB_t<NumberType> r = *this;

				if (r.pos.x < boundries.pos.x) {
					r.size.x += boundries.pos.x - r.pos.x;
					r.pos.x = boundries.pos.x;
				}

				if (r.pos.y < boundries.pos.y) {
					r.size.y += boundries.pos.y - r.pos.y;
					r.pos.y = boundries.pos.y;
				}

				float wdiff = (r.pos.x + r.size.x) - (boundries.pos.x + boundries.size.x);
				if (wdiff > 0) r.size.x -= wdiff;

				float hdiff = (r.pos.y + r.size.y) - (boundries.pos.y + boundries.size.y);
				if (hdiff > 0) r.size.y -= hdiff;

				return r;
			}
		};

		using AABBd = AABB_t<double>;
		using AABBf = AABB_t<float>;
		using AABBi = AABB_t<int>;
		using AABB = AABBf;
	}
}