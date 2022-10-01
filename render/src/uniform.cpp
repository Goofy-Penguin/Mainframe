#include <mainframe/render/uniform.h>

#include <iostream>
#include <fstream>
#include <GL/glew.h>

namespace mainframe {
	namespace render {
		bool Uniform::isValid() {
			return index != -1;
		}

		void Uniform::set(int value) {
#ifndef MAINFRAME_EGL
			glUniform1i(index, value);
#endif
		}

		void Uniform::set(const int* values, size_t count) {
#ifndef MAINFRAME_EGL
			glUniform1iv(index, count, values);
#endif
		}

		void Uniform::set(float value) {
#ifndef MAINFRAME_EGL
			glUniform1f(index, value);
#endif
		}

		void Uniform::set(const float* values, size_t count) {
#ifndef MAINFRAME_EGL
			glUniform1fv(index, count, values);
#endif
		}

		void Uniform::set(const mainframe::math::Vector2& value) {
#ifndef MAINFRAME_EGL
			glUniform2f(index, value.x, value.y);
#endif
		}

		void Uniform::set(const mainframe::math::Vector2* values, size_t count) {
#ifndef MAINFRAME_EGL
			glUniform2fv(index, count, &values->x);
#endif
		}

		void Uniform::set(const mainframe::math::Vector3& value) {
#ifndef MAINFRAME_EGL
			glUniform3f(index, value.x, value.y, value.z);
#endif
		}

		void Uniform::set(const mainframe::math::Vector3* values, size_t count) {
#ifndef MAINFRAME_EGL
			glUniform3fv(index, count, &values->x);
#endif
		}

		void Uniform::set(const mainframe::render::Color& value) {
#ifndef MAINFRAME_EGL
			glUniform4f(index, value.r, value.g, value.b, value.a);
#endif
		}

		void Uniform::set(const mainframe::render::Color* values, size_t count) {
#ifndef MAINFRAME_EGL
			glUniform4fv(index, count, reinterpret_cast<const float*>(values));
#endif
		}

		void Uniform::set(const mainframe::math::Matrix& value) {
#ifndef MAINFRAME_EGL
			glUniformMatrix4fv(index, 1, GL_TRUE, reinterpret_cast<const GLfloat*>(value.getValues().data()));
#endif
		}

		void Uniform::set(const mainframe::math::Matrix* values, size_t count) {
#ifndef MAINFRAME_EGL
			// create flattend matrix-float values array
			std::vector<float> floats;
			floats.resize(count * 16);

			// copy over each matrix into flattend array
			for (size_t i = 0; i < count; i++) {
				std::copy_n(values[i].getValues().data(), 16, floats.begin() + i * 16);
			}

			glUniformMatrix4fv(index, count, GL_FALSE, &floats.front());
#endif
		}

		int Uniform::getlocation() {
			return index;
		}
	}
}
