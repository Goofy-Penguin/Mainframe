﻿#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <complex>
#include <fmt/format.h>

namespace mainframe {
	namespace render {
		template <class NumberType>
		class Color_t {
		private:
			using ColorType = Color_t<NumberType>;

		public:
			NumberType r = 0, g = 0, b = 0, a = 1;

			static Color_t<NumberType> RGBAHex(uint32_t x) {
				if constexpr(std::is_same<NumberType, float>::value) {
					return {
						((x >> 24) & 0xFF) / 255.0f,
						((x >> 16) & 0xFF) / 255.0f,
						((x >> 8) & 0xFF) / 255.0f,
						((x) & 0xFF) / 255.0f
					};
				} else {
					return {
						((x >> 24) & 0xFF),
						((x >> 16) & 0xFF),
						((x >> 8) & 0xFF),
						((x) & 0xFF)
					};
				}
			}

			static Color_t<NumberType> RGBHex(uint32_t x) {
				if constexpr(std::is_same<NumberType, float>::value) {
					return {
						((x >> 16) & 0xFF) / 255.0f,
						((x >> 8) & 0xFF) / 255.0f,
						((x) & 0xFF) / 255.0f
					};
				} else {
					return {
						((x >> 16) & 0xFF),
						((x >> 8) & 0xFF),
						((x) & 0xFF)
					};
				}
			}

			template<class ReturnType>
			Color_t<ReturnType> cast() const {
				if constexpr(std::is_same<NumberType, int>::value) {
					return {
						static_cast<ReturnType>(r) / 255.0f,
						static_cast<ReturnType>(g) / 255.0f,
						static_cast<ReturnType>(b) / 255.0f,
						static_cast<ReturnType>(a) / 255.0f
					};
				} else {
					return {
						static_cast<ReturnType>(r * 255.0f),
						static_cast<ReturnType>(g * 255.0f),
						static_cast<ReturnType>(b * 255.0f),
						static_cast<ReturnType>(a * 255.0f)
					};
				}
			}

			static ColorType debug(int index) {
				ColorType cols[]{
					ColorType(255, 82, 82, 255),// rgb(255, 82, 82)
					ColorType(51, 217, 178, 255),// rgb(51, 217, 178)
					ColorType(52, 172, 224, 255),// rgb(52, 172, 224)
					ColorType(112, 111, 211, 255),// rgb(112, 111, 211)
					ColorType(179, 55, 113, 255),// rgb(179, 55, 113)
					ColorType(252, 66, 123, 255),// rgb(252, 66, 123)
					ColorType(241, 196, 15, 255),// rgb(241, 196, 15)

					ColorType(88, 177, 159, 255),// rgb(88, 177, 159)
					ColorType(27, 20, 100, 255),// rgb(27, 20, 100)
					ColorType(87, 88, 187, 255),// rgb(87, 88, 187)
					ColorType(255, 255, 255, 255) // rgb(255, 255, 255)
				};

				auto col = cols[index % (sizeof(cols) / sizeof(ColorType))];
				if constexpr(std::is_same<NumberType, int>::value) {
					return col;
				} else {
					return col / 255.0f;
				}
			}

			Color_t() = default;
			Color_t(NumberType _r, NumberType _g = 0, NumberType _b = 0, NumberType _a = -1) : r(_r), g(_g), b(_b), a(_a) {
                if (a == -1) {
                    if constexpr(std::is_same<NumberType, int>::value) {
                        a = 255;
                    } else {
                        a = 1.0f;
                    }
                }
            }

			NumberType distance(const ColorType& other) const {
				return static_cast<NumberType>(std::sqrt(((r - other.r) * (r - other.r)) + ((g - other.g) * (g - other.g)) + ((b - other.b) * (b - other.b)) + ((a - other.a) * (a - other.a))));
			}

			NumberType length() const {
				return static_cast<NumberType>(std::sqrt(std::pow(r, 2) + std::pow(g, 2) + std::pow(b, 2) + std::pow(a, 2)));
			}

			ColorType normalized() const {
				return (*this) / length();
			}

			std::string toString() const {
				return fmt::format("{},{},{},{}", r, g, b, a);
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

			ColorType operator* (const ColorType& other) const {
				return ColorType(r * other.r, g * other.g, b * other.b, a * other.a);
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
				return ColorType(r * scale, g * scale, b * scale, a * scale);
			}

			ColorType operator/ (NumberType scale) const {
				return ColorType(r / scale, g / scale, b / scale, a / scale);
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

		using Colorf = Color_t<float>;
		using Colori = Color_t<int>;
		using Color = Colorf;

		template <class NumberType>
		class Colors_t {
		private:
			using ColorType = Color_t<NumberType>;

		public:
			inline const static ColorType Black = ColorType(0, 0, 0);
			inline const static ColorType White = ColorType(1, 1, 1);

			inline const static ColorType Gray = ColorType(0.81f, 0.8f, 0.75f);
			inline const static ColorType DarkGray = ColorType(0.51f, 0.50f, 0.47f);

			inline const static ColorType Red = ColorType(1, 0.32f, 0.32f);
			inline const static ColorType Green = ColorType(0.14f, 0.87f, 0.50f);
			inline const static ColorType Blue = ColorType(0.2f, 0.67f, 0.87f);
			inline const static ColorType Orange = ColorType(1, 0.47f, 0.24f);
			inline const static ColorType Yellow = ColorType(1, 0.69f, 0.25f);
			inline const static ColorType Purple = ColorType(0.43f, 0.43f, 0.82f);

			inline const static ColorType Transparent = ColorType(0, 0, 0, 0);
		};

		using Colorsf = Colors_t<float>;
		using Colorsi = Colors_t<int>;
		using Colors = Colorsf;
	}
}
