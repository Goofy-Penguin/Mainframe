#include <mainframe/game/window.h>

#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <Windows.h>
#undef max
#undef min
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#define GLFWHANDLE reinterpret_cast<GLFWwindow*>(handle)

namespace mainframe {
	namespace game {
		Window& glfwHandleToWindow(GLFWwindow* ptr) {
			return *static_cast<Window*>(glfwGetWindowUserPointer(ptr));
		}

		void Window::callbacks_resize(GLFWwindow* whandle, int width, int height) {
			glViewport(0, 0, width, height);

			auto& window = glfwHandleToWindow(whandle);
			window.onResize(window, {width, height});
		}

		void Window::callbacks_key(GLFWwindow* whandle, int key, int scancode, int action, int mods) {
			if (key < 0) return;

			auto& window = glfwHandleToWindow(whandle);
			window.onKey(window,
						 static_cast<unsigned int>(key),
						 static_cast<unsigned int>(scancode),
						 static_cast<unsigned int>(action),
						 static_cast<unsigned int>(mods)
			);

			if (action == GLFW_REPEAT) return;
			window.keysIn[key] = action != GLFW_RELEASE ? 1 : 0;
		}

		void Window::callbacks_mouseKey(GLFWwindow* whandle, int button, int action, int mods) {
			if (button < 0) return;

			auto& window = glfwHandleToWindow(whandle);
			auto pos = window.getMousePos();

			window.onMouseKey(window,
							  pos,
							  static_cast<unsigned int>(button),
							  static_cast<unsigned int>(action),
							  static_cast<unsigned int>(mods)
			);

			window.mouseIn[button] = action == GLFW_PRESS ? 1 : 0;
		}

		void Window::callbacks_char(GLFWwindow* whandle, unsigned int ch) {
			auto& window = glfwHandleToWindow(whandle);
			window.onChar(window, ch);
		}

		void Window::callbacks_scroll(GLFWwindow* whandle, double x, double y) {
			auto& window = glfwHandleToWindow(whandle);

			auto size = window.getSize();
			auto pos = window.getMousePos();

			if (pos.x < 0 || pos.y < 0 || pos.x > size.x || pos.y > size.y) return;

			window.onScroll(window, pos, {static_cast<int>(x * 10), static_cast<int>(y * 10)});
		}

		void Window::callbacks_mouseMove(GLFWwindow* whandle, double x, double y) {
			auto& window = glfwHandleToWindow(whandle);
			window.onMouseMove(window, {static_cast<int>(x), static_cast<int>(y)});
		}

		void Window::callbacks_focus(GLFWwindow* whandle, int focus) {
			auto& window = glfwHandleToWindow(whandle);
			window.hasFocus = focus == 1;
		}

		void Window::addOnResize(OnResizeCallback::Func callback) {
			onResize += callback;
		}

		void Window::addOnChar(OnCharCallback::Func callback) {
			onChar += callback;
		}

		void Window::addOnKey(OnKeyCallback::Func callback) {
			onKey += callback;
		}

		void Window::addOnMouseKey(OnMouseKeyCallback::Func callback) {
			onMouseKey += callback;
		}

		void Window::addOnMouseMove(OnMouseMoveCallback::Func callback) {
			onMouseMove += callback;
		}

		void Window::addOnScroll(OnScrollCallback::Func callback) {
			onScroll += callback;
		}

		void Window::addOnFocus(OnFocusCallback::Func callback) {
			onFocus += callback;
		}

		bool Window::setTitle(const std::string& title) {
			glfwSetWindowTitle(GLFWHANDLE, title.c_str());
			return true;
		}

		Window::~Window() {
			close();
		}

		math::Vector2i Window::getFrameSize() const {
			math::Vector2i ret;
			glfwGetFramebufferSize(GLFWHANDLE, &ret.x, &ret.y);

			return ret;
		}

		math::Vector2i Window::getSize() const {
			math::Vector2i ret;
			glfwGetWindowSize(GLFWHANDLE, &ret.x, &ret.y);

			return ret;
		}

		math::Vector2i Window::getMousePos() const {
			double x, y;
			glfwGetCursorPos(GLFWHANDLE, &x, &y);

			return math::Vector2i(static_cast<int>(std::floor(x)), static_cast<int>(std::floor(y)));
		}

		void Window::setPos(const math::Vector2i& pos) {
			glfwSetWindowPos(GLFWHANDLE, pos.x, pos.y);
		}

		void Window::setMonitor(const Monitor& monitor, const math::Vector2i& pos, const math::Vector2i& size) {
			glfwSetWindowMonitor(GLFWHANDLE, monitor.handle, pos.x, pos.y, size.x, size.y, monitor.refreshRate);
		}

		void Window::setMousePos(const math::Vector2i& pos) {
			glfwSetCursorPos(GLFWHANDLE, static_cast<double>(pos.x), static_cast<double>(pos.y));
		}

		void Window::waitForEvents() const {
			glfwWaitEvents();
		}

		void* Window::getHandle() const {
			return handle;
		}

		void Window::blink() {
			glfwRequestWindowAttention(GLFWHANDLE);
		}

		void Window::maximize() {
			glfwMaximizeWindow(GLFWHANDLE);
		}

		void Window::minimize() {
			glfwIconifyWindow(GLFWHANDLE);
		}

		void Window::focus() {
			glfwFocusWindow(GLFWHANDLE);
		}

		bool Window::getFocus() {
			return hasFocus;
		}

		void Window::use() {
			glfwMakeContextCurrent(GLFWHANDLE);
		}

		void Window::swapBuffer() const {
			glfwSwapBuffers(GLFWHANDLE);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Window::setSize(const math::Vector2i& size) {
			glfwSetWindowSize(GLFWHANDLE, size.x, size.y);
		}

		void Window::setCallbacks() {
			glfwSetKeyCallback(GLFWHANDLE, callbacks_key);
			glfwSetCharCallback(GLFWHANDLE, callbacks_char);
			glfwSetScrollCallback(GLFWHANDLE, callbacks_scroll);
			glfwSetWindowSizeCallback(GLFWHANDLE, callbacks_resize);
			glfwSetWindowFocusCallback(GLFWHANDLE, callbacks_focus);
			glfwSetCursorPosCallback(GLFWHANDLE, callbacks_mouseMove);
			glfwSetMouseButtonCallback(GLFWHANDLE, callbacks_mouseKey);
		}

		void Window::pollEvents() {
			glfwPollEvents();
		}

		void Window::close() {
			if (handle == nullptr) return;

			glfwDestroyWindow(GLFWHANDLE);
			handle = nullptr;
		}

		bool Window::getShouldClose() const {
			if (handle == nullptr) return false;
			return glfwWindowShouldClose(GLFWHANDLE);
		}

		void Window::setShouldClose(bool close) const {
			if (handle == nullptr) return;
			glfwSetWindowShouldClose(GLFWHANDLE, close ? 1 : 0);
		}

		void Window::setProperty(int key, int value) {
			properties[key] = value;
		}

		void Window::restore() {
			glfwRestoreWindow(GLFWHANDLE);
		}

		void Window::center() {
			auto mon = getMonitor();
			setPos(mon.pos + mon.size / 2 - getSize() / 2);
		}

		void Window::waitEvents() {
			glfwWaitEvents();
		}

		Monitor Window::getMonitor() {
			// https://stackoverflow.com/a/31526753
			int nmonitors, i;
			int wx, wy, ww, wh;
			int mx, my, mw, mh;
			int overlap, bestoverlap;
			GLFWmonitor* bestmonitor;
			GLFWmonitor** monitors;
			const GLFWvidmode* mode;

			bool primary = false;
			bestoverlap = 0;
			bestmonitor = NULL;

			glfwGetWindowPos(GLFWHANDLE, &wx, &wy);
			glfwGetWindowSize(GLFWHANDLE, &ww, &wh);
			monitors = glfwGetMonitors(&nmonitors);

			for (i = 0; i < nmonitors; i++) {
				mode = glfwGetVideoMode(monitors[i]);
				glfwGetMonitorPos(monitors[i], &mx, &my);
				mw = mode->width;
				mh = mode->height;

				overlap =
					std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
					std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

				if (bestoverlap < overlap) {
					bestoverlap = overlap;
					primary = i == 0;
					bestmonitor = monitors[i];
				}
			}

			if (bestmonitor == nullptr) return {};
			return {bestmonitor};
		}


#ifdef WIN32
		void __stdcall OpenglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
#else
		void OpenglDebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {
#endif

			if (severity != GL_DEBUG_SEVERITY_HIGH) return;

			printf("OPENGL DEBUG OUTPUT: source 0x%x, type 0x%x, id %d, severity 0x%x, message %s\n", source, type, id, severity, message);
		}

		bool Window::initGlew() {
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
			return true;
		}

		bool Window::create(int w, int h, const std::string& title, bool fullscreen, bool resizable, int monitorId) {
			Desktop::initGlfw();

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
				w = mode->width;
				h = mode->height;
				if (isborderless) {
					fullscreen = false;
				}
			} else if ((w >= mode->width || h >= mode->height)) {
				if (!isborderless) fullscreen = true;
				w = mode->width;
				h = mode->height;
			}

			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			properties[GLFW_RESIZABLE] = resizable ? GL_TRUE : GL_FALSE;
			for (const auto& pair : properties) {
				glfwWindowHint(pair.first, pair.second);
			}

			auto glfwHandle = glfwCreateWindow(w, h, title.c_str(), fullscreen ? mon : nullptr, nullptr);
			glfwSetWindowUserPointer(glfwHandle, this);

			handle = glfwHandle;

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
			HANDLE hIcon = LoadIconW(GetModuleHandleW(nullptr), L"GLFW_ICON");
			if (!hIcon) {
				// No user-provided icon found, load default icon
				hIcon = LoadIcon(nullptr, IDI_WINLOGO);
			}

			HWND hwnd = glfwGetWin32Window(glfwHandle);
			::SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			::SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
#endif

			setCallbacks();

			glfwMakeContextCurrent(glfwHandle);
			glfwSwapInterval(1); // 0 == infinite FPS, 1 == 60, 2 == 30

			if (!initGlew()) {
				close();
				return false;
			}

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			glViewport(0, 0, w, h);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			return true;
		}
	}
}