#include <mainframe/render/viabuffer.h>
#include <GL/glew.h>

namespace mainframe {
	namespace render {
		VIABuffer::VIABuffer() {
#ifndef MAINFRAME_EGL
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
#endif
		}

		VIABuffer::~VIABuffer() {
#ifndef MAINFRAME_EGL
			glDeleteBuffers(1, &ebo);
			glDeleteBuffers(1, &vbo);
			glDeleteVertexArrays(1, &vao);
#endif
		}

		unsigned int VIABuffer::getVao() {
			return vao;
		}

		unsigned int VIABuffer::getVbo() {
			return vbo;
		}

		unsigned int VIABuffer::getEbo() {
			return ebo;
		}
	}
}