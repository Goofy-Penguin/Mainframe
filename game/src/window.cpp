#include <window.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#ifdef WIN32
#include <Windows.h>
#endif

namespace mainframe {
	namespace game {
		void error_callback(int error, const char* description) {
			printf("GLFW error %d: '%s'\n", error, description);
		}

		bool Window::create(int w, int h, bool fullscreen, bool resizable, int monitorId) {
			if (glfwInit() != GL_TRUE) {
				printf("glfwInit failed!\n");
				return false;
			}

			glfwSetErrorCallback(error_callback);

			GLFWmonitor* mon = nullptr;
			bool isborderless = properties.find(GLFW_DECORATED) != properties.end() && properties[GLFW_DECORATED] > 0;

			if (monitorId >= 0) {
				int monitorscount = 0;
				auto monitors = glfwGetMonitors(&monitorscount);
				if (monitorId < monitorscount) {
					mon = monitors[monitorId];
					properties[GLFW_VISIBLE] = 0;
				}
			}

			if (mon == nullptr) {
				mon = glfwGetPrimaryMonitor();
			}

			if (mon == nullptr) {
				printf("glfwGetPrimaryMonitor failed!\n");
				return false;
			}

			const GLFWvidmode* mode = glfwGetVideoMode(mon);

			if (fullscreen) {
				w = (unsigned int)mode->width;
				h = (unsigned int)mode->height;
				if (isborderless) {
					fullscreen = false;
				}
			} else if ((w >= (unsigned int)mode->width || h >= (unsigned int)mode->height)) {
				if (!isborderless) fullscreen = true;
				w = (unsigned int)mode->width;
				h = (unsigned int)mode->height;
			}

			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			properties[GLFW_RESIZABLE] = resizable ? GL_TRUE : GL_FALSE;
			for (const auto& pair : properties) {
				glfwWindowHint(pair.first, pair.second);
			}

			auto glfwHandle = glfwCreateWindow(w, h, "", fullscreen ? mon : nullptr, nullptr);
			if (glfwHandle == nullptr) {
				printf("glfwCreateWindow failed!\n");
				return false;
			}

			if (!fullscreen) {
				int monx = 0;
				int mony = 0;
				int monw = 0;
				int monh = 0;

				glfwGetMonitorPos(mon, &monx, &mony);
				glfwSetWindowPos(glfwHandle, monx + mode->width / 2 - w / 2, mony + mode->height / 2 - h / 2);
				glfwShowWindow(glfwHandle);
			}

#ifdef WIN32
			HANDLE hIcon = LoadIconW(GetModuleHandleW(NULL), L"GLFW_ICON");
			if (!hIcon) {
				// No user-provided icon found, load default icon
				hIcon = LoadIconW(NULL, IDI_WINLOGO);
			}

			HWND hwnd = glfwGetWin32Window(glfwHandle);
			::SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			::SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
#endif

			setCallbacks();

			glfwMakeContextCurrent(glfwHandle);
			glViewport(0, 0, w, h);
			glfwSwapInterval(1); // 0 == infinite FPS, 1 == 60, 2 == 30

			glewExperimental = GL_TRUE;
			GLenum err = glewInit();
			if (err != GLEW_OK) {
				printf("glewInit failed!\n");
				printf("Error: %s\n", glewGetErrorString(err));
				return false;
			}

			GLenum currentError = glGetError(); // Glew always throws a silly error, this is to filter that.
			if (!(currentError == GL_NO_ERROR || currentError == GL_INVALID_ENUM)) {
				printf("something's wrong with glew...%x\n", currentError);
				return false;
			}

			glDebugMessageCallback(OpenglDebugCallback, nullptr);
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			this->HasFocus = true;
			return true;
		}
	}
}