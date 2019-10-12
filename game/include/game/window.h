#pragma once

#include <string>
#include <map>
#include <utils/event.hpp>

namespace mainframe {
	namespace game {
		class Window;

		using OnResizeCallback = mainframe::utils::Event<Window&, int, int>;

		class Window {
		private:
			std::map<int, unsigned char> keysIn;
			std::map<int, unsigned char> mouseIn;
			std::map<int, int> properties;
			void* handle = nullptr;
			bool hasFocus = false;

			void setCallbacks();

			OnResizeCallback onResize;
		public:
			// @param callback: std::function<void(Window& win, int width, int height)>
			void addOnResize(OnResizeCallback::Func callback);

			bool create(int width, int height, bool fullscreen = false, bool resizable = false, int monitorId = -1);
		};
	}
}