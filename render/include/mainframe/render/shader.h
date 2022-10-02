#pragma once

#include <mainframe/math/vector2.h>
#include <mainframe/math/vector3.h>
#include <mainframe/math/matrix.h>
#include <mainframe/render/uniform.h>
#include <string>

namespace mainframe::render {
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
		bool attach(const std::string& path, int mode);

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
