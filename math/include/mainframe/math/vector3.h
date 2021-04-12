#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <mainframe/math/vector2.h>

namespace mainframe {
	namespace math {
		template <class NumberType>
		class Vector3_t {
		private:
			using VecType = Vector3_t<NumberType>;

		public:
			NumberType x = 0, y = 0, z = 0;

			Vector3_t() = default;
			Vector3_t(NumberType val) : x(val), y(val), z(val) {}
			Vector3_t(NumberType _x, NumberType _y, NumberType _z = 0) : x(_x), y(_y), z(_z) {}

			//TODO: fix to use vector2 or update function to work in 3d space
			NumberType distanceToLine(const VecType& point_a, const VecType& point_b) {
				VecType ap = *this - point_a;
				VecType ab = point_b - point_a;
				NumberType ab2 = ab.x * ab.x + ab.y * ab.y;
				NumberType ap_ab = ap.x * ab.x + ap.y * ab.y;

				NumberType t = ap_ab / ab2;
				if (t < 0.0f) t = 0.0f;
				else if (t > 1.0f) t = 1.0f;

				VecType closest_point = point_a + ab * t;
				return closest_point.distance(*this);
			}

			NumberType distance(const VecType& other) const {
				return std::sqrt(((x - other.x) * (x - other.x)) + ((y - other.y) * (y - other.y)) + ((z - other.z) * (z - other.z)));
			}

			NumberType length() const {
				return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
			}

			VecType normalized() const {
				return (*this) / length();
			}

			Vector2_t<NumberType> xy() const {
				return {x, y};
			}

			VecType xzy() const {
				return {x, z, y};
			}

			VecType floor() const {
				return {std::floor(x), std::floor(y), std::floor(z)};
			}

			VecType getRotated(const VecType& ang, const VecType& orgin) const {
				// TODO: when importing matrix
				//Matrix rotmat = Matrix::CreateRotationz(ang.x) * Matrix::CreateRotationy(ang.z) * Matrix::CreateRotationx(ang.y) * Matrix::CreateTranslation(orgin);
				//return rotmat.Translate(*this);

				return {};
			}

			VecType cross(const VecType& other) const {
				VecType retVal;

				retVal.x = y * other.z - z * other.y;
				retVal.y = z * other.x - x * other.z;
				retVal.z = x * other.y - y * other.x;

				return retVal;
			}

			NumberType dot(const VecType& other) const {
				return x * other.x + y * other.y + z * other.z;
			}

			VecType operator- (const VecType& other) const {
				return VecType(x - other.x, y - other.y, z - other.z);
			}

			VecType operator+ (const VecType& other) const {
				return VecType(x + other.x, y + other.y, z + other.z);
			}

			VecType operator* (const VecType& other) const {
				return VecType(x * other.x, y * other.y, z * other.z);
			}

			VecType operator/ (const VecType& other) const {
				return VecType(x / other.x, y / other.y, z / other.z);
			}

			VecType operator- (NumberType other) const {
				return VecType(x - other, y - other, z - other);
			}

			VecType operator+ (NumberType other) const {
				return VecType(x + other, y + other, z + other);
			}

			VecType operator* (NumberType other) const {
				return VecType(x * other, y * other, z * other);
			}

			VecType operator/ (NumberType other) const {
				return VecType(x / other, y / other, z / other);
			}

			VecType& operator-= (const VecType& other) {
				x = x - other.x;
				y = y - other.y;
				z = z - other.z;
				return *this;
			}

			VecType& operator+= (const VecType& other) {
				x = x + other.x;
				y = y + other.y;
				z = z + other.z;
				return *this;
			}

			VecType& operator*= (const VecType& other) {
				x = x * other.x;
				y = y * other.y;
				z = z * other.z;
				return *this;
			}

			VecType& operator/= (const VecType& other) {
				x = x / other.x;
				y = y / other.y;
				z = z / other.z;
				return *this;
			}

			VecType& operator-= (NumberType other) {
				x = x - other;
				y = y - other;
				z = z - other;
				return *this;
			}

			VecType& operator+= (NumberType other) {
				x = x + other;
				y = y + other;
				z = z + other;
				return *this;
			}

			VecType& operator*= (NumberType other) {
				x = x * other;
				y = y * other;
				z = z * other;
				return *this;
			}

			VecType& operator/= (NumberType other) {
				x = x / other;
				y = y / other;
				z = z / other;
				return *this;
			}

			VecType operator-() const {
				return VecType(-x, -y, -z);
			}

			bool operator== (const VecType& other) const {
				return x == other.x && y == other.y && z == other.z;
			}

			bool operator!= (const VecType& other) const {
				return !operator==(other);
			}

			VecType fromAngles(NumberType pitch, NumberType yaw) {
				return VecType(
					std::sin(pitch) * std::cos(yaw),
					std::sin(pitch) * std::sin(yaw),
					std::cos(pitch)
				);
			}
		};

		using Vector3d = Vector3_t<double>;
		using Vector3f = Vector3_t<float>;
		using Vector3i = Vector3_t<int>;
		using Vector3 = Vector3f;
	}
}