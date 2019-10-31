#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace mainframe {
	namespace render {
		template <class NumberType>
		class Color_t {
		private:
			using ColorType = Color_t<NumberType>;

		public:
			NumberType r = 0, g = 0, b = 0, a = 1;

			Color_t() = default;
			Color_t(NumberType _r, NumberType _g = 0, NumberType _b = 0, NumberType _a = 1) : r(_r), g(_g), b(_b), a(_a) {}

			NumberType distance(const ColorType& other) const {
				return std::sqrt(((r - other.r) * (r - other.r)) + ((g - other.g) * (g - other.g)) + ((b - other.b) * (b - other.b)) + ((a - other.a) * (a - other.a)));
			}

			NumberType length() const {
				return std::sqrt(std::pow(r, 2) + std::pow(g, 2) + std::pow(b, 2) + std::pow(a, 2));
			}

			ColorType normalized() const {
				return (*this) / length();
			}

			NumberType dot(const ColorType& other) const {
				return r * other.r + g * other.g + b * other.b + a * other.a;
			}

			ColorType operator- (const ColorType& other) const {
				return ColorType(r - other.r, g - other.g, b - other.b, a - other.a);
			}

			ColorType operator+ (const ColorType& other) const {
				return ColorType(r + other.r, g + other.g, b + other.b, a + other.a);
			}

			ColorType operator* (const ColorType& scale) const {
				return ColorType(r * scale.r, g * scale.g, b * scale.b, a * other.a);
			}

			ColorType operator/ (const ColorType& other) const {
				return ColorType(r / other.r, g / other.g, b / other.b, a / other.a);
			}

			ColorType operator- (NumberType other) const {
				return ColorType(r - other, g - other, b - other, a - other);
			}

			ColorType operator+ (NumberType other) const {
				return ColorType(r + other, g + other, b + other, a + other);
			}

			ColorType operator* (NumberType scale) const {
				return ColorType(r * scale, g * scale, b * scale, a * other);
			}

			ColorType operator/ (NumberType scale) const {
				return ColorType(r / scale, g / scale, b / scale, a / other);
			}

			ColorType& operator-= (const ColorType& other) {
				r -= other.r;
				g -= other.g;
				b -= other.b;
				a -= other.a;
				return *this;
			}

			ColorType& operator+= (const ColorType& other) {
				r += other.r;
				g += other.g;
				b += other.b;
				a += other.a;
				return *this;
			}

			ColorType& operator*= (const ColorType& other) {
				r *= other.r;
				g *= other.g;
				b *= other.b;
				a *= other.a;
				return *this;
			}

			ColorType& operator/= (const ColorType& other) {
				r /= other.r;
				g /= other.g;
				b /= other.b;
				a /= other.a;
				return *this;
			}

			ColorType& operator-= (NumberType other) {
				r -= other;
				g -= other;
				b -= other;
				a -= other;
				return *this;
			}

			ColorType& operator+= (NumberType other) {
				r += other;
				g += other;
				b += other;
				a += other;
				return *this;
			}

			ColorType& operator*= (NumberType other) {
				r *= other;
				g *= other;
				b *= other;
				a *= other;
				return *this;
			}

			ColorType& operator/= (NumberType other) {
				r /= other;
				g /= other;
				b /= other;
				a /= other;
				return *this;
			}

			ColorType operator-() const {
				return ColorType(-r, -g, -b, -a);
			}

			bool operator== (const ColorType& other) const {
				return r == other.r && g == other.g && b == other.b && a == other.a;
			}

			bool operator!= (const ColorType& other) const {
				return !operator==(other);
			}
		};

		using Colord = Color_t<double>;
		using Colorf = Color_t<float>;
		using Colori = Color_t<int>;
		using Color = Colorf;

		template <class NumberType>
		class Colors_t {
		private:
			using ColorType = Color_t<NumberType>;

		public:
			inline const static ColorType Black = ColorType(0, 0, 0);
			inline const static ColorType Gray = ColorType(0.3f, 0.3f, 0.3f);
			inline const static ColorType DarkGray = ColorType(0.1f, 0.1f, 0.1f);
			inline const static ColorType White = ColorType(1, 1, 1);
			inline const static ColorType Red = ColorType(1, 0, 0);
			inline const static ColorType Green = ColorType(0, 1, 0);
			inline const static ColorType Blue = ColorType(0, 0, 1);
			inline const static ColorType Orange = ColorType(1, 0.6f, 0);
			inline const static ColorType Yellow = ColorType(1, 1, 0);
			inline const static ColorType Transparent = ColorType(0, 0, 0, 0);
		};

		using Colorsd = Colors_t<double>;
		using Colorsf = Colors_t<float>;
		using Colorsi = Colors_t<int>;
		using Colors = Colorsf;
	}
}