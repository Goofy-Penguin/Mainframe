#pragma once

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

			template<typename T, typename U>
			struct is_same : std::false_type {};

			template<typename T>
			struct is_same<T, T> : std::true_type {};

			template<typename T, typename U>
			static constexpr bool eqTypes() { return is_same<T, U>::value; }

		public:
			NumberType x = 0, y = 0;

			Vector2_t() = default;
			Vector2_t(NumberType _x, NumberType _y = 0) : x(_x), y(_y) {}

			NumberType distance(const VecType& other) const {
				return std::sqrt(((x - other.x) * (x - other.x)) + ((y - other.y) * (y - other.y)));
			}

			NumberType length() const {
				return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
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

			VecType lerp(const VecType& other, NumberType timestep) {
				VecType ret;

				ret.x = x + (other.x - x) * timestep;
				ret.y = y + (other.y - y) * timestep;

				return ret;
			}

			NumberType atan2() {
				return std::atan2(y, x);
			}

			static VecType cosSin(NumberType radians) {
				return VecType(std::cos(radians), std::sin(radians));
			}

			VecType rotateAroundOrigin(NumberType rads, const VecType& origin) {
				if (rads == 0) return *this;

				VecType u = *this - origin;
				if (u == VecType()) return u;

				u = cosSin(u.atan2() + rads) * u.length();
				u.x += origin.x;
				u.y += origin.y;

				return u;
			}

			VecType yx() const {
				return VecType(y, x);
			}

			VecType operator- (const VecType& other) const {
				return VecType(x - other.x, y - other.y);
			}

			VecType operator+ (const VecType& other) const {
				return VecType(x + other.x, y + other.y);
			}

			VecType operator* (const VecType& other) const {
				return VecType(x * other.x, y * other.y);
			}

			VecType operator/ (const VecType& other) const {
				return VecType(x / other.x, y / other.y);
			}

			VecType operator* (NumberType scale) const {
				return VecType(x * scale, y * scale);
			}

			VecType operator/ (NumberType scale) const {
				return VecType(x / scale, y / scale);
			}

			VecType operator+ (NumberType other) const {
				return VecType(x + other, y + other);
			}

			VecType operator- (NumberType other) const {
				return VecType(x - other, y - other);
			}

			VecType& operator-= (const VecType& other) {
				x = x - other.x;
				y = y - other.y;
				return *this;
			}

			VecType& operator+= (const VecType& other) {
				x = x + other.x;
				y = y + other.y;
				return *this;
			}

			VecType& operator*= (const VecType& other) {
				x = x * other.x;
				y = y * other.y;
				return *this;
			}

			VecType& operator/= (const VecType& other) {
				x = x / other.x;
				y = y / other.y;
				return *this;
			}

			VecType& operator*= (NumberType other) {
				x = x * other;
				y = y * other;
				return *this;
			}

			VecType& operator+= (NumberType other) {
				x = x + other;
				y = y + other;
				return *this;
			}

			VecType& operator/= (NumberType other) {
				x = x / other;
				y = y / other;
				return *this;
			}

			VecType operator-() const {
				return VecType(-x, -y);
			}

			bool operator== (const VecType& other) const {
				return x == other.x && y == other.y;
			}

			bool operator!= (const VecType& other) const {
				return !operator==(other);
			}
		};

		using Vector2d = Vector2_t<double>;
		using Vector2f = Vector2_t<float>;
		using Vector2i = Vector2_t<int>;
		using Vector2 = Vector2f;
	}
}