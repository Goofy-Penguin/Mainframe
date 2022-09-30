#pragma once

#include <mainframe/math/vector2.h>
#include <mainframe/math/vector3.h>
#include <mainframe/math/matrix.h>
#include <mainframe/render/color.h>
#include <string>

namespace mainframe {
	namespace render {
		class Uniform {
			int index = -1;

		public:
			Uniform(int _index) : index(_index) {}
			Uniform() = default;

			bool isValid();
			int getlocation();

			void set(int value);
			void set(const int* values, size_t count);

			void set(float value);
			void set(const float* values, size_t count);

			void set(const mainframe::math::Vector2& value);
			void set(const mainframe::math::Vector2* values, size_t count);

			void set(const mainframe::math::Vector3& value);
			void set(const mainframe::math::Vector3* values, size_t count);

			void set(const mainframe::math::Matrix& value);
			void set(const mainframe::math::Matrix* values, size_t count);

			void set(const mainframe::render::Color& value);
			void set(const mainframe::render::Color* values, size_t count);
		};
	}
}
