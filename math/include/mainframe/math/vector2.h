﻿#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <cmath>

namespace mainframe {
	namespace math {
		template <class NumberType>
		class Vector2_t {
		private:
			using VecType = Vector2_t<NumberType>;

		public:
			NumberType x = 0, y = 0;

			Vector2_t() = default;
			Vector2_t(NumberType val) : x(static_cast<NumberType>(val)), y(static_cast<NumberType>(val)) {}

			template <class TX, class TY>
			Vector2_t(TX _x, TY _y) : x(static_cast<NumberType>(_x)), y(static_cast<NumberType>(_y)) {}

			template<class T>
			Vector2_t(Vector2_t<T> other) : x(static_cast<NumberType>(other.x)), y(static_cast<NumberType>(other.y)) {}

			static VecType zero() { return VecType(); }
			static VecType one() { return VecType(1, 1); }

			NumberType distance(const VecType& other) const {
				return std::sqrt(((x - other.x) * (x - other.x)) + ((y - other.y) * (y - other.y)));
			}

			NumberType length() const {
				return static_cast<NumberType>(std::sqrt(std::pow(x, 2) + std::pow(y, 2)));
			}

			NumberType angle(const VecType& target) const {
				return -std::atan2(x - target.x, y - target.y);
			}

			VecType normalized() const {
				NumberType l = length();
				return l == 0 ? VecType() : (*this) / l;
			}

			NumberType dot(const VecType& other) const {
				return x * other.x + y * other.y;
			}

			NumberType cross(const VecType& other) const {
				return x * other.y - y * other.x;
			}

			VecType abs() const {
				return {std::abs(x), std::abs(y)};
			}

			VecType lerp(const VecType& other, NumberType timestep) const {
				VecType ret;

				ret.x = x + (other.x - x) * timestep;
				ret.y = y + (other.y - y) * timestep;

				return ret;
			}

			VecType floor() const {
				return { std::floor(x), std::floor(y) };
			}

			VecType round() const {
				return { std::round(x), std::round(y) };
			}

			VecType ceil() const {
				return { std::ceil(x), std::ceil(y) };
			}

			VecType clampVec(const VecType& min, const VecType& max) const {
				return {
					std::clamp(x, min.x, max.x),
					std::clamp(y, min.y, max.y)
				};
			}

			VecType clamp(NumberType min, NumberType max) const {
				return {
					std::clamp(x, min, max),
					std::clamp(y, min, max)
				};
			}

			NumberType atan2() const {
				return std::atan2(y, x);
			}

			static VecType cosSin(NumberType radians) {
				return VecType(std::cos(radians), std::sin(radians));
			}

			bool isNaN() const {
				return std::isnan(x) || std::isnan(y);
			}

			VecType rotateAroundOrigin(NumberType rads, const VecType& origin) const {
				if (rads == 0) return *this;

				VecType u = *this - origin;
				if (u == VecType()) return *this;

				u = cosSin(u.atan2() + rads) * static_cast<NumberType>(u.length());
				u.x += origin.x;
				u.y += origin.y;

				return u;
			}

			VecType yx() const {
				return VecType(y, x);
			}

			template<class ReturnType>
			Vector2_t<ReturnType> cast() const {
				return {static_cast<ReturnType>(x), static_cast<ReturnType>(y)};
			}

			// numberic typed operators
			VecType operator- (NumberType other) const { return VecType(x - other, y - other); }
			VecType operator+ (NumberType other) const { return VecType(x + other, y + other); }
			VecType operator* (NumberType other) const { return VecType(x * other, y * other); }
			VecType operator/ (NumberType other) const { return VecType(x / other, y / other); }

			VecType& operator-= (NumberType other) { x = x - other; y = y - other; return *this; }
			VecType& operator+= (NumberType other) { x = x + other; y = y + other; return *this; }
			VecType& operator*= (NumberType other) { x = x * other; y = y * other; return *this; }
			VecType& operator/= (NumberType other) { x = x / other; y = y / other; return *this; }

			bool operator== (NumberType other) const { return x == other && y == other; }
			bool operator!= (NumberType other) const { return !operator==(other); }
			bool operator< (NumberType other) const { return x < other && y < other; }
			bool operator<= (NumberType other) const { return x <= other && y <= other; }
			bool operator> (NumberType other) const { return x > other && y > other; }
			bool operator>= (NumberType other) const { return x >= other && y >= other; }


			// vector typed operators
			VecType operator- (const VecType& other) const { return VecType(x - other.x, y - other.y); }
			VecType operator+ (const VecType& other) const { return VecType(x + other.x, y + other.y); }
			VecType operator* (const VecType& other) const { return VecType(x * other.x, y * other.y); }
			VecType operator/ (const VecType& other) const { return VecType(x / other.x, y / other.y); }

			VecType& operator-= (const VecType& other) { x = x - other.x; y = y - other.y; return *this; }
			VecType& operator+= (const VecType& other) { x = x + other.x; y = y + other.y; return *this; }
			VecType& operator*= (const VecType& other) { x = x * other.x; y = y * other.y; return *this; }
			VecType& operator/= (const VecType& other) { x = x / other.x; y = y / other.y; return *this; }

			bool operator== (const VecType& other) const { return x == other.x && y == other.y; }
			bool operator!= (const VecType& other) const { return !operator==(other); }
			bool operator< (const VecType& other) const { return x < other.x && y < other.y; }
			bool operator<= (const VecType& other) const { return x <= other.x && y <= other.y; }
			bool operator> (const VecType& other) const { return x > other.x && y > other.y; }
			bool operator>= (const VecType& other) const { return x >= other.x && y >= other.y; }

			VecType operator-() const { return VecType(-x, -y); }
		};

		using Vector2d = Vector2_t<double>;
		using Vector2f = Vector2_t<float>;
		using Vector2i = Vector2_t<int>;
		using Vector2 = Vector2f;
	}
}
