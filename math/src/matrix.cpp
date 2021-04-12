#include <mainframe/math/matrix.h>
#include <mainframe/numbers/pi.h>

namespace mainframe {
	namespace math {
		Matrix::Matrix() {
			values = std::array<float, 16>{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
		}

		Matrix::Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44) {
			values = std::array<float, 16>{m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44};
		}

		Matrix::Matrix(float v) {
			values = std::array<float, 16>{v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v};
		}

		Matrix::Matrix(const Matrix& other) {
			values = other.values;
		}

		Matrix::Matrix(const std::array<float, 16>& values_) {
			values = values_;
		}

		Matrix::Matrix(Matrix&& other) {
			values = other.values;
			other.values = {};
		}

		Matrix Matrix::lerp(const Matrix& o, float timestep) {
			Matrix ret;

			for (size_t i = 0; i < values.size(); i++) {
				ret.values[i] = this->values[i] + (o.values[i] - this->values[i]) * timestep;
			}

			return ret;
		}

		Matrix Matrix::mirror() {
			Matrix ret;

			for (int x = 0; x < 4; x++) {
				for (int y = 0; y < 4; y++) {
					ret.values[x * 4 + y] = this->values[y * 4 + x];
				}
			}

			return ret;
		}

		Matrix Matrix::inverted() {
			// TODO: translate to english

			float num17 = (values[10] * values[15] - values[11] * values[14]);
			float num18 = (values[9] * values[15] - values[11] * values[13]);
			float num19 = (values[9] * values[14] - values[10] * values[13]);
			float num20 = (values[8] * values[15] - values[11] * values[12]);
			float num21 = (values[8] * values[14] - values[10] * values[12]);
			float num22 = (values[8] * values[13] - values[9] * values[12]);
			float num23 = (values[5] * num17 - values[6] * num18 + values[7] * num19);
			float num24 = -(values[4] * num17 - values[6] * num20 + values[7] * num21);
			float num25 = (values[4] * num18 - values[5] * num20 + values[7] * num22);
			float num26 = -(values[4] * num19 - values[5] * num21 + values[6] * num22);
			float num27 = (1.0f / (values[0] * num23 + values[1] * num24 + values[2] * num25 + values[3] * num26));

			float num28 = (values[6] * values[15] - values[7] * values[14]);
			float num29 = (values[5] * values[15] - values[7] * values[13]);
			float num30 = (values[5] * values[14] - values[6] * values[13]);
			float num31 = (values[4] * values[15] - values[7] * values[12]);
			float num32 = (values[4] * values[14] - values[6] * values[12]);
			float num33 = (values[4] * values[13] - values[5] * values[12]);

			float num34 = (values[6] * values[11] - values[7] * values[10]);
			float num35 = (values[5] * values[11] - values[7] * values[9]);
			float num36 = (values[5] * values[10] - values[6] * values[9]);
			float num37 = (values[4] * values[11] - values[7] * values[8]);
			float num38 = (values[4] * values[10] - values[6] * values[8]);
			float num39 = (values[4] * values[9] - values[5] * values[8]);

			return {
				num23 * num27,
				-(values[1] * num17 - values[2] * num18 + values[3] * num19) * num27,
				(values[1] * num28 - values[2] * num29 + values[3] * num30) * num27,
				-(values[1] * num34 - values[2] * num35 + values[3] * num36) * num27,
				num24 * num27,
				(values[0] * num17 - values[2] * num20 + values[3] * num21) * num27,
				-(values[0] * num28 - values[2] * num31 + values[3] * num32) * num27,
				(values[0] * num34 - values[2] * num37 + values[3] * num38) * num27,
				num25 * num27,
				-(values[0] * num18 - values[1] * num20 + values[3] * num22) * num27,
				 (values[0] * num29 - values[1] * num31 + values[3] * num33) * num27,
				 -(values[0] * num35 - values[1] * num37 + values[3] * num39) * num27,
				 num26 * num27,
				 (values[0] * num19 - values[1] * num21 + values[2] * num22) * num27,
				 -(values[0] * num30 - values[1] * num32 + values[2] * num33) * num27,
				 (values[0] * num36 - values[1] * num38 + values[2] * num39) * num27
			};
		}

		Matrix& Matrix::operator= (const Matrix& rhs) {
			if (&rhs == this) { return *this; }

			values = rhs.values;
			return *this;
		}

		Matrix Matrix::createRotationX(float rotation) {
			Matrix retVal;
			retVal.values[5] = cos(rotation);
			retVal.values[6] = -sin(rotation);
			retVal.values[9] = sin(rotation);
			retVal.values[10] = cos(rotation);
			return retVal;
		}

		Matrix Matrix::createRotationY(float rotation) {
			Matrix retVal;
			retVal.values[0] = cos(rotation);
			retVal.values[2] = sin(rotation);
			retVal.values[8] = -sin(rotation);
			retVal.values[10] = cos(rotation);
			return retVal;
		}

		Matrix Matrix::createRotationZ(float rotation) {
			Matrix retVal;
			retVal.values[0] = cos(rotation);
			retVal.values[1] = -sin(rotation);
			retVal.values[4] = sin(rotation);
			retVal.values[5] = cos(rotation);
			return retVal;
		}

		Matrix Matrix::createScale(float scale) {
			return Matrix::createScale(scale, scale, scale);
		}

		Matrix Matrix::createScale(float x, float y, float z) {
			Matrix retVal;
			retVal.values[0] = x;
			retVal.values[5] = y;
			retVal.values[10] = z;
			return retVal;
		}

		Matrix Matrix::createScale(const Vector3& scale) {
			Matrix retVal;
			retVal.values[0] = scale.x;
			retVal.values[5] = scale.y;
			retVal.values[10] = scale.z;
			return retVal;
		}

		Matrix Matrix::createTranslation(float x, float y, float z) {
			Matrix retVal;
			retVal.values[3] = x;
			retVal.values[7] = y;
			retVal.values[11] = z;
			return retVal;
		}

		Matrix Matrix::createTranslation(const Vector3& pos) {
			return createTranslation(pos.x, pos.y, pos.z);
		}

		Vector3 Matrix::getTranslation() const {
			return translate(math::Vector3 {});
		}

		Vector3 Matrix::getScale() const {
			return Vector3(values[0], values[5], values[10]);
		}

		Vector3 Matrix::getRotation() const {
			// https://gamedev.stackexchange.com/a/50968
			if (values[0] == 1.0f || values[0] == -1.0f) {
				return {
					std::atan2(values[2], values[11]),
					0,
					0
				};
			}

			return {
				std::atan2(values[2], values[11]),
				std::asin(values[4]),
				std::atan2(values[6], values[5]),
			};
		}


		Matrix Matrix::createLookAt(float eyex, float eyey, float eyez, float targetx, float targety, float targetz, float upx, float upy, float upz) {
			return createLookAt({eyex, eyey, eyez}, {targetx, targety, targetz}, {upx, upy, upz});
		}

		Matrix Matrix::createLookAt(const Vector3& eyePos, const Vector3& targetPos, const Vector3& upPos) {
			auto zAxis = (eyePos.xzy() - targetPos.xzy()).normalized();
			auto xAxis = upPos.cross(zAxis).normalized();
			auto yAxis = zAxis.cross(xAxis).normalized();

			Matrix orientation = Matrix(
				xAxis.x, yAxis.x, zAxis.x, 0,
				xAxis.y, yAxis.y, zAxis.y, 0,
				xAxis.z, yAxis.z, zAxis.z, 0,
				0, 0, 0, 1
			);

			Matrix translation = Matrix(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				-eyePos.x, -eyePos.y, -eyePos.z, 1
			);

			// Combine the orientation and translation to compute the view matrix
			return (translation * orientation);
		}

		Matrix Matrix::createPerspective(float fovy, float aspect, float zNear, float zFar) {
			float f = tan(numbers::pi<float> / 2 - 0.5f * fovy);
			float rangeInv = 1.0f / (zNear - zFar);

			Matrix retVal(0);

			retVal.values[0] = f / aspect;
			retVal.values[5] = f;
			retVal.values[10] = (zNear + zFar) * rangeInv;
			retVal.values[11] = zNear * zFar * rangeInv * 2.0f;
			retVal.values[14] = -1.0;

			return retVal;
		}

		Matrix Matrix::createOrthographic(float l, float r, float t, float b, float n, float f) {
			Matrix retVal;

			retVal.values[0] = 2 / (r - l);
			retVal.values[5] = 2 / (t - b);
			retVal.values[10] = -2 / (f - n);
			//retVal.values[10] = -2 / (f - n);
			retVal.values[15] = 1;

			/*retVal.values[12] = -((r + l) / (r - l));
			retVal.values[13] = -((t + b) / (t - b));
			retVal.values[14] = -((f + n) / (f - n));*/

			return retVal;
		}

		const std::array<float, 16>& Matrix::getValues() const {
			return values;
		}

		Vector3 Matrix::translate(float x, float y, float z) const {
			Vector3 retVal = Vector3();
			retVal.x = values[0] * x + values[1] * y + values[2] * z + values[3];
			retVal.y = values[4] * x + values[5] * y + values[6] * z + values[7];
			retVal.z = values[8] * x + values[9] * y + values[10] * z + values[11];
			return retVal;
		}

		Vector3 Matrix::translate(const Vector3& input) const {
			return translate(input.x, input.y, input.z);
		}

		Vector4 Matrix::translate(float x, float y, float z, float w) const {
			Vector4 retVal;
			retVal.x = values[0] * x + values[1] * y + values[2] * z + values[3] * w;
			retVal.y = values[4] * x + values[5] * y + values[6] * z + values[7] * w;
			retVal.z = values[8] * x + values[9] * y + values[10] * z + values[11] * w;
			retVal.w = values[12] * x + values[13] * y + values[14] * z + values[15] * w;
			return retVal;
		}

		Vector4 Matrix::translate(const Vector4& input) const {
			return translate(input.x, input.y, input.z, input.w);
		}

		Matrix Matrix::operator*(const Matrix& other) const {
			Matrix retVal;

			retVal.values[0] = values[0] * other.values[0] + values[1] * other.values[4] + values[2] * other.values[8] + values[3] * other.values[12];
			retVal.values[1] = values[0] * other.values[1] + values[1] * other.values[5] + values[2] * other.values[9] + values[3] * other.values[13];
			retVal.values[2] = values[0] * other.values[2] + values[1] * other.values[6] + values[2] * other.values[10] + values[3] * other.values[14];
			retVal.values[3] = values[0] * other.values[3] + values[1] * other.values[7] + values[2] * other.values[11] + values[3] * other.values[15];

			retVal.values[4] = values[4] * other.values[0] + values[5] * other.values[4] + values[6] * other.values[8] + values[7] * other.values[12];
			retVal.values[5] = values[4] * other.values[1] + values[5] * other.values[5] + values[6] * other.values[9] + values[7] * other.values[13];
			retVal.values[6] = values[4] * other.values[2] + values[5] * other.values[6] + values[6] * other.values[10] + values[7] * other.values[14];
			retVal.values[7] = values[4] * other.values[3] + values[5] * other.values[7] + values[6] * other.values[11] + values[7] * other.values[15];

			retVal.values[8] = values[8] * other.values[0] + values[9] * other.values[4] + values[10] * other.values[8] + values[11] * other.values[12];
			retVal.values[9] = values[8] * other.values[1] + values[9] * other.values[5] + values[10] * other.values[9] + values[11] * other.values[13];
			retVal.values[10] = values[8] * other.values[2] + values[9] * other.values[6] + values[10] * other.values[10] + values[11] * other.values[14];
			retVal.values[11] = values[8] * other.values[3] + values[9] * other.values[7] + values[10] * other.values[11] + values[11] * other.values[15];

			retVal.values[12] = values[12] * other.values[0] + values[13] * other.values[4] + values[14] * other.values[8] + values[15] * other.values[12];
			retVal.values[13] = values[12] * other.values[1] + values[13] * other.values[5] + values[14] * other.values[9] + values[15] * other.values[13];
			retVal.values[14] = values[12] * other.values[2] + values[13] * other.values[6] + values[14] * other.values[10] + values[15] * other.values[14];
			retVal.values[15] = values[12] * other.values[3] + values[13] * other.values[7] + values[14] * other.values[11] + values[15] * other.values[15];

			return retVal;
		}

		void Matrix::debugPrint() {
			printf("Matrix object at %p\n", this);
			printf("m11: %+f m12: %+f m13: %+f m14: %+f \n"
				   "m21: %+f m22: %+f m23: %+f m24: %+f \n"
				   "m31: %+f m32: %+f m33: %+f m34: %+f \n"
				   "m41: %+f m42: %+f m43: %+f m44: %+f \n",
				   values[0], values[1], values[2], values[3],
				   values[4], values[5], values[6], values[7],
				   values[8], values[9], values[10], values[11],
				   values[12], values[13], values[14], values[15]
			);
		}
	}
}
