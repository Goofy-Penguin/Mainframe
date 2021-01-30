#pragma once

#include <array>
#include <mainframe/math/vector3.h>

namespace mainframe {
	namespace math {
		class Matrix {
			std::array<float, 16> values {0};

		public:
			static Matrix createRotationX(float rotation);
			static Matrix createRotationY(float rotation);
			static Matrix createRotationZ(float rotation);
			static Matrix createScale(float scale);
			static Matrix createScale(float scaleX, float scaleY, float scaleZ);
			static Matrix createScale(const Vector3& scale);
			static Matrix createTranslation(float x, float y, float z);
			static Matrix createTranslation(const Vector3& pos);
			static Matrix createLookAt(float eyex, float eyey, float eyez, float targetx, float targety, float targetz, float upx, float upy, float upz);
			static Matrix createLookAt(const Vector3& eyePos, const Vector3& targetPos, const Vector3& up);
			static Matrix createPerspective(float fovy, float aspect, float zNear, float zFar);
			static Matrix createOrthographic(float l, float r, float t, float b, float n, float f);

			~Matrix();
			Matrix();
			Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
			Matrix(float values);
			Matrix(std::array<float, 16> values);

			Matrix(const Matrix& other);
			Matrix(Matrix&& other);

			void debugPrint();

			Vector3 translate(float x, float y, float z) const;
			Vector3 translate(const Vector3& input) const;

			Vector3 getTranslation() const;
			Vector3 getScale() const;

			Matrix lerp(const Matrix& other, float timestep);
			Matrix inverted();

			// to switch between column and row type
			Matrix mirror();

			Matrix& operator= (const Matrix& rhs);
			Matrix operator*(const Matrix& other) const;
			void operator*=(const Matrix& other) { *this = *this * other; }
		};
	}
}
