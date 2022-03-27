#pragma once

#include <string>
#include <chrono>

namespace mainframe::game {
	class Engine {
	private:
		bool shouldShutdown = false;
		unsigned int tps = 66;
		unsigned int fps = 60;
		bool runningSlow = false;
		std::chrono::milliseconds deadlockBreaker = std::chrono::milliseconds(500);
		std::chrono::nanoseconds delayBetweenTicks;
		std::chrono::nanoseconds delayBetweenFrames;

	public:
		// one time initializer
		virtual void init();

		// mark quit flag and allow for `isQuiting()` for clean exit threaded
		virtual void quit();

		// poll window and input events
		virtual void pollEvents();

		// called on a fixed timestep
		virtual void update(float deltaTime, long long gameTime);

		// called acordingly with the FPS lock
		virtual void draw(const double alpha);

		// starts the game loop and blocks until quit is called and is handled
		virtual void run();

		// sets maximum time of backlog for update and draw calls. Once backlog will be cleared
		// and will start skipping cycles
		virtual void setBreakerTime(const std::chrono::milliseconds& timeBeforeBreaker);
		virtual const std::chrono::milliseconds& getBreakerTime();

		// sets the update rate per second
		virtual void setTPS(unsigned int ticksPerSecond);
		virtual unsigned int getTPS();

		// sets the draw rate per second
		virtual void setFPS(unsigned int framesPerSecond);
		virtual unsigned int getFPS();

		// returns true after quit() is called
		bool isQuitting();

		// are we running behind on updates
		bool isRunningSlow();
	};
}
