#pragma once

#include <string>
#include <mainframe/math/vector2.h>

struct GLFWmonitor;

namespace mainframe {
	namespace game {
		class Monitor {
		public:
			int id = -1;
			GLFWmonitor* handle = nullptr;
			bool primary = false;
			math::Vector2i pos;
			math::Vector2i size;

			Monitor() = default;
			Monitor(GLFWmonitor* handle_);
		};

		class Desktop {
			static bool inited;

		public:
			static std::vector<Monitor> getMonitors();
			static Monitor getMonitor(int  id = -1);

			static bool initGlfw();
			static bool initGlew();
			static void terminateGlfw();
		};
	}
}