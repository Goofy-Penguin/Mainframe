#pragma once

#include <mainframe/math/vector2.h>
#include <mainframe/math/vector3.h>
#include <mainframe/math/matrix.h>
#include <string>

namespace mainframe {
	namespace render {
		class Uniform {
		private:
			int index = -1;
		public:
			Uniform(int _index) : index(_index) {}
			Uniform() = default;

			void set(int value);
			void set(float value);
			void set(const mainframe::math::Vector2& value);
			void set(const mainframe::math::Vector3& value);
			void set(const mainframe::math::Matrix& value);
		};

		class ShaderAttached {
		public:
			std::string filename;
			int handle;

			ShaderAttached() = default;
			ShaderAttached(const std::string& file, int _handle) : filename(file), handle(_handle) {}
		};

		class Shader {
			int programHandle = -1;

		public:
			std::vector<ShaderAttached> attached;

			Shader();
			~Shader();

			void use() const;
			void link();
			bool attachRaw(const std::string& text, int mode);
			bool attach(const std::string& file, int mode);
			void cleanup();

			int getHandle() const;

			// Setting vectors
			void setUniform(std::string sName, const mainframe::math::Vector2* vectors, int count = 1);
			void setUniform(std::string sName, const mainframe::math::Vector3* vectors, int count = 1);

			// Setting floats
			void setUniform(std::string sName, const float* values, int count = 1);
			void setUniform(std::string sName, const float value);

			// Setting 4x4 matrices
			//void setUniform(std::string sName, const TomatoLib::Matrix& mMatrix);

			// Setting integers
			void setUniform(std::string sName, const int* values, int count = 1);
			void setUniform(std::string sName, const int value);

			Uniform getUniform(const std::string& sName);
		};
	}
}