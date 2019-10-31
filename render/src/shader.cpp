#include <mainframe/render/shader.h>

#include <iostream>
#include <fstream>
#include <GL/glew.h>

namespace mainframe {
	namespace render {
		Shader::Shader() {
		}

		Shader::~Shader() {
			cleanup();
		}

		void Shader::cleanup() {
#ifndef MAINFRAME_EGL
			if (programHandle == -1) return;

			for (size_t i = 0; i < attached.size(); i++) {
				glDeleteShader(attached[i].handle);
			}

			glDeleteProgram(programHandle);

			programHandle = -1;
			attached.clear();
#endif
		}

		int Shader::getHandle() const {
			return programHandle;
		}

		bool Shader::attach(const std::string& file, int mode) {
#ifndef MAINFRAME_EGL
			std::ifstream infile;
			infile.open(file, std::ifstream::binary);

			if (!infile.is_open()) {
				infile.close();
				return false;
			}

			if (programHandle == -1) {
				programHandle = glCreateProgram();
			}

			infile.seekg(0, std::ios::end);
			size_t file_size_in_byte = (size_t)infile.tellg();

			std::vector<char> data; // used to store text data
			data.resize(file_size_in_byte + 1);
			infile.seekg(0, std::ios::beg);

			infile.read(&data[0], file_size_in_byte);
			data[file_size_in_byte] = 0;

			//Cast to a const char for the gl function
			const char* fileDataConst = (const char*)&data[0];

			//Create new shader, set the source, and compile it
			GLuint handle = glCreateShader(mode);

			glShaderSource(handle, 1, &fileDataConst, 0);
			glCompileShader(handle);

			infile.close();

			//Check if compile is succesfull
			GLint isSuccesfullyCompiled = 0;
			glGetShaderiv(handle, GL_COMPILE_STATUS, &isSuccesfullyCompiled);
			if (isSuccesfullyCompiled == GL_FALSE) {
				//Get info log length
				GLint maxLength = 0;
				glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> errBuffer;
				errBuffer.resize(maxLength + 1);

				//Allocate data for log
				glGetShaderInfoLog(handle, maxLength, &maxLength, errBuffer.data());

				glDeleteShader(handle);

				throw std::runtime_error(errBuffer.data());
			}

			glAttachShader(programHandle, handle);

			attached.emplace_back(file, handle);
#endif
			return true;
		}

		bool Shader::attachRaw(const std::string& text, int mode) {
#ifndef MAINFRAME_EGL
			if (programHandle == -1) {
				programHandle = glCreateProgram();
			}

			//Create new shader, set the source, and compile it
			GLuint handle = glCreateShader(mode);
			const char* str = text.c_str();
			glShaderSource(handle, 1, (const GLchar**)&str, 0);
			glCompileShader(handle);

			//Check if compile is succesfull
			GLint isSuccesfullyCompiled = 0;
			glGetShaderiv(handle, GL_COMPILE_STATUS, &isSuccesfullyCompiled);
			if (isSuccesfullyCompiled == GL_FALSE) {
				//Get info log length
				GLint maxLength = 0;
				glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);

				//Allocate data for log
				std::string log;
				log.reserve(maxLength);
				glGetShaderInfoLog(handle, maxLength, &maxLength, (GLchar*)log.c_str());

				glDeleteShader(handle);

				printf("Shader compile error: %s\n", log.c_str());
				return false;
			}

			glAttachShader(programHandle, handle);

			attached.emplace_back("raw", handle);
			return true;
#endif

			return false;
		}

		void Shader::use() const {
#ifndef MAINFRAME_EGL
			glUseProgram(programHandle);
#endif
		}

		void Shader::link() {
#ifndef MAINFRAME_EGL
			glLinkProgram(programHandle);

			GLint isLinked = 0;
			glGetProgramiv(programHandle, GL_LINK_STATUS, &isLinked);
			if (isLinked == GL_FALSE) {
				GLint maxLength = 0;
				glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog;
				infoLog.resize(maxLength + 1);

				glGetProgramInfoLog(programHandle, maxLength, &maxLength, infoLog.data());
				this->cleanup();

				throw std::runtime_error("Error linking gl program, " + std::string(infoLog.data()));
			}
#endif
		}// Setting floats

		void Shader::setUniform(std::string sName, const float* fValues, int iCount) {
#ifndef MAINFRAME_EGL
			int iLoc = glGetUniformLocation(programHandle, sName.c_str());
			glUniform1fv(iLoc, iCount, fValues);
#endif
		}

		void Shader::setUniform(std::string sName, const float fValue) {
#ifndef MAINFRAME_EGL
			int iLoc = glGetUniformLocation(programHandle, sName.c_str());
			glUniform1fv(iLoc, 1, &fValue);
#endif
		}

		// Setting vectors

		void Shader::setUniform(std::string sName, const mainframe::math::Vector2* vectors, int count) {
#ifndef MAINFRAME_EGL
			int iLoc = glGetUniformLocation(programHandle, sName.c_str());
			glUniform2fv(iLoc, count, (GLfloat*)vectors);
#endif
		}

		void Shader::setUniform(std::string sName, const mainframe::math::Vector3* vectors, int count) {
#ifndef MAINFRAME_EGL
			int iLoc = glGetUniformLocation(programHandle, sName.c_str());
			glUniform3fv(iLoc, count, (GLfloat*)vectors);
#endif
		}

		// Setting integers

		void Shader::setUniform(std::string sName, const int* values, int count) {
#ifndef MAINFRAME_EGL
			int iLoc = glGetUniformLocation(programHandle, sName.c_str());
			glUniform1iv(iLoc, count, values);
#endif
		}

		void Shader::setUniform(std::string sName, const int iValue) {
#ifndef MAINFRAME_EGL
			int iLoc = glGetUniformLocation(programHandle, sName.c_str());
			glUniform1i(iLoc, iValue);
#endif
		}

		Uniform Shader::getUniform(const std::string& sName) {
#ifndef MAINFRAME_EGL
			return Uniform(glGetUniformLocation(programHandle, sName.c_str()));
#else
			return {};
#endif
		}

		void Uniform::set(int value) {
#ifndef MAINFRAME_EGL
			glUniform1i(index, value);
#endif
		}

		void Uniform::set(float value) {
#ifndef MAINFRAME_EGL
			glUniform1f(index, value);
#endif
		}

		void Uniform::set(const mainframe::math::Vector2& value) {
#ifndef MAINFRAME_EGL
			glUniform2f(index, value.x, value.y);
#endif
		}

		void Uniform::set(const mainframe::math::Vector3& value) {
#ifndef MAINFRAME_EGL
			glUniform3f(index, value.x, value.y, value.z);
#endif
		}
	}
}