#pragma once

#include <string>

namespace mainframe::game {
	class Engine {
	private:
		bool shouldShutdown = false;
		unsigned int tick = 66;

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
		virtual void setTick(unsigned int tick);

		// returns true after quit() is called
		bool isQuitting();
	};
}
