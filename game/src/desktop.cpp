#include <mainframe/game/desktop.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace mainframe {
	namespace game {
		bool Desktop::inited = false;

		void fillMonitor(Monitor& obj) {
			glfwGetMonitorPos(obj.handle, &obj.pos.x, &obj.pos.y);

			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			obj.size = {mode->width, mode->height};
		}

		Monitor::Monitor(GLFWmonitor* handle_) : handle(handle_) {
			fillMonitor(*this);
		}

		std::vector<Monitor> Desktop::getMonitors() {
			int count = 0;
			auto monitors = glfwGetMonitors(&count);

			std::vector<Monitor> ret;
			for (int i = 0; i < count; i++) {
				Monitor obj = {monitors[i]};
				obj.id = i;
				obj.primary = i == 0;
				ret.push_back(obj);
			}

			return ret;
		}

		Monitor Desktop::getMonitor(int id) {
			GLFWmonitor* m = nullptr;
			if (id == -1) {
				m = glfwGetPrimaryMonitor();
			} else {
				int count = 0;
				auto monitors = glfwGetMonitors(&count);

				if (id >= count || id < 0) throw std::runtime_error("index out of bounds");
				m = monitors[id];
			}

			Monitor obj = {m};
			obj.primary = id == -1;
			obj.id = obj.primary ? 0 : id;

			return obj;
		}

		void error_callback(int error, const char* description) {
			printf("GLFW error %d: '%s'\n", error, description);
		}

		bool Desktop::initGlfw() {
			if (glfwInit() != GL_TRUE) return false;

			if (inited) return true;

			glfwSetErrorCallback(error_callback);

			inited = true;
			return true;
		}

		void Desktop::terminateGlfw() {
			glfwTerminate();
			inited = false;
		}
	}
}