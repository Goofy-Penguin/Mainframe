#pragma once

#include <string>
#include <map>
#include <mainframe/utils/event.hpp>
#include <mainframe/math/vector2.h>

struct GLFWwindow;

namespace mainframe {
	namespace game {
		class Window;

		using OnFocusCallback = mainframe::utils::Event<Window&, bool>;
		using OnCharCallback = mainframe::utils::Event<Window&, unsigned int>;
		using OnResizeCallback = mainframe::utils::Event<Window&, const math::Vector2i&>;
		using OnScrollCallback = mainframe::utils::Event<Window&, const math::Vector2i&>;
		using OnMouseMoveCallback = mainframe::utils::Event<Window&, const math::Vector2i&>;
		using OnKeyCallback = mainframe::utils::Event<Window&, unsigned int, unsigned int, unsigned int, unsigned int>;
		using OnMouseKeyCallback = mainframe::utils::Event<Window&, const math::Vector2i&, unsigned int, unsigned int, unsigned int>;

		class Window {
		private:
			std::map<unsigned int, unsigned char> keysIn;
			std::map<unsigned int, unsigned char> mouseIn;
			std::map<int, int> properties;
			void* handle = nullptr;
			bool hasFocus = false;

			void setCallbacks();

			static void callbacks_focus(GLFWwindow* whandle, int focus);
			static void callbacks_char(GLFWwindow* whandle, unsigned int ch);
			static void callbacks_scroll(GLFWwindow* whandle, double x, double y);
			static void callbacks_mouseMove(GLFWwindow* whandle, double x, double y);
			static void callbacks_resize(GLFWwindow* whandle, int width, int height);
			static void callbacks_mouseKey(GLFWwindow* whandle, int button, int action, int mods);
			static void callbacks_key(GLFWwindow* whandle, int key, int scancode, int action, int mods);

			OnKeyCallback onKey;
			OnCharCallback onChar;
			OnFocusCallback onFocus;
			OnResizeCallback onResize;
			OnScrollCallback onScroll;
			OnMouseKeyCallback onMouseKey;
			OnMouseMoveCallback onMouseMove;

		public:
			// @param callback: std::function<void(Window& win, const math::Vector2i& size)>
			void addOnResize(OnResizeCallback::Func callback);

			// @param callback: std::function<void(Window& win, unsigned int character)>
			void addOnChar(OnCharCallback::Func callback);

			// @param callback: std::function<void(Window& win, unsigned int key, unsigned int scancode, unsigned int action, unsigned int mods)>
			void addOnKey(OnKeyCallback::Func callback);

			// @param callback: std::function<void(Window& win, const math::Vector2i& location, unsigned int button, unsigned int action, unsigned int mods)>
			void addOnMouseKey(OnMouseKeyCallback::Func callback);

			// @param callback: std::function<void(Window& win, const math::Vector2i& location)>
			void addOnMouseMove(OnMouseMoveCallback::Func callback);

			// @param callback: std::function<void(Window& win, const math::Vector2i& offset)>
			void addOnScroll(OnScrollCallback::Func callback);

			// @param callback: std::function<void(Window& win, bool hasFocus)>
			void addOnFocus(OnFocusCallback::Func callback);

			bool create(int width, int height, const std::string& title = "", bool fullscreen = false, bool resizable = false, int monitorId = -1);
			bool shouldClose() const;
			void close();

			bool getFocus();
			void focus();
			void blink();

			void minimize();
			void maximize();

			math::Vector2i getFrameSize() const;
			math::Vector2i getSize() const;
			math::Vector2i getPos() const;
			math::Vector2i getMousePos() const;
			void* getHandle() const;

			bool setTitle(const std::string& title);
			void setSize(const math::Vector2i& size);
			void setPos(const math::Vector2i& pos);
			void setMousePos(const math::Vector2i& pos);

			void waitForEvents() const;
			void swapBuffer() const;

			void use();

			static void pollEvents();
		};
	}
}