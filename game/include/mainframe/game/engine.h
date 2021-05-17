#pragma once

#include <string>

namespace mainframe {
	namespace game {
		class Engine {
		private:
			bool shouldShutdown = false;

			int currentFPS = 0;
			float currentFPSTime = 0;
			float targetFPS = 60;

		public:
			virtual void init();
			virtual void quit();

			// called as much as possible, free from FPS lock
			virtual void tick();

			// called acordingly with the FPS lock
			virtual void update();

			// called acordingly with the FPS lock
			virtual void draw();

			// starts the game loop and blocks until quit is called and is handled
			virtual void run();

			// returns true after quit() is called
			bool isQuitting();

			void setFPS(float count);
		};
	}
}