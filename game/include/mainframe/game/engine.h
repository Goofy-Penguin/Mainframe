#pragma once

#include <string>

namespace mainframe {
	namespace game {
		class Engine {
		private:
			bool shouldShutdown = false;
			float timeStep = 0.25f;

		public:
			virtual void init();
			virtual void quit();

			// called on a fixed timestep
			virtual void update(float deltaTime);

			// called acordingly with the FPS lock
			virtual void draw();

			// starts the game loop and blocks until quit is called and is handled
			virtual void run();

			// sets the game tickrate
			virtual void setTimestep(float timeStep);

			// returns true after quit() is called
			bool isQuitting();
		};
	}
}
