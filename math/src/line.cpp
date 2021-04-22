#include <mainframe/math/line.h>

namespace mainframe {
	namespace math {
		Line::Line(const Vector2& a_, const Vector2& b_) : a(a), b(b) {}

		bool Line::intersects(const Line& other) const {
			// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
			auto const& p = this->a;
			auto const& q = other.a;
			auto const r = this->b - p;
			auto const s = other.b - q;

			auto const t = (q - p).cross(s) / (r.cross(s));
			auto const u = (q - p).cross(r) / (r.cross(s));

			return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
		}

		bool Line::intersects(const Line& other, Vector2& point) const {
			// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
			auto const& p = this->a;
			auto const& q = other.a;
			auto const r = this->b - p;
			auto const s = other.b - q;

			auto const t = (q - p).cross(s) / (r.cross(s));
			auto const u = (q - p).cross(r) / (r.cross(s));

			point = {p + r * t};

			return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
		}

		float Line::length() const {
			return this->a.distance(this->b);
		}

		Vector2 Line::lerp(float timestep) {
			return this->a.lerp(this->b, timestep);
		}

		bool Line::operator==(const Line& other) const {
			return this->a == other.a && this->b == other.b;
		}

		bool Line::operator!=(const Line& other) const {
			return !(*this == other);
		}


		// float
		Line Line::operator-(float other) const {
			return {this->a - other, this->b - other};
		}

		Line Line::operator+(float other) const {
			return {this->a + other, this->b + other};
		}

		Line Line::operator/(float other) const {
			return {this->a / other, this->b / other};
		}

		Line Line::operator*(float other) const {
			return {this->a * other, this->b * other};
		}

		Line& Line::operator-=(float other) {
			this->a -= other;
			this->b -= other;

			return *this;
		}

		Line& Line::operator+=(float other) {
			this->a += other;
			this->b += other;

			return *this;
		}

		Line& Line::operator*=(float other) {
			this->a *= other;
			this->b *= other;

			return *this;
		}

		Line& Line::operator/=(float other) {
			this->a /= other;
			this->b /= other;

			return *this;
		}

		// Vector2
		Line Line::operator-(const Vector2& other) const {
			return {this->a - other, this->b - other};
		}

		Line Line::operator+(const Vector2& other) const {
			return {this->a + other, this->b + other};
		}

		Line Line::operator/(const Vector2& other) const {
			return {this->a / other, this->b / other};
		}

		Line Line::operator*(const Vector2& other) const {
			return {this->a * other, this->b * other};
		}

		Line& Line::operator-=(const Vector2& other) {
			this->a -= other;
			this->b -= other;

			return *this;
		}

		Line& Line::operator+=(const Vector2& other) {
			this->a += other;
			this->b += other;

			return *this;
		}

		Line& Line::operator*=(const Vector2& other) {
			this->a *= other;
			this->b *= other;

			return *this;
		}

		Line& Line::operator/=(const Vector2& other) {
			this->a /= other;
			this->b /= other;

			return *this;
		}
	}
}
