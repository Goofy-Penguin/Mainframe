#include <mainframe/game/engine.h>

#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std::literals;

using Clock = std::chrono::steady_clock;


namespace mainframe {
	namespace game {
		void Engine::init() {}

		void Engine::pollEvents() {}
		void Engine::update(float deltaTime, long long gameTime) {}

		void Engine::draw(const double alpha) {}
		void Engine::run() {
			delayBetweenTicks = std::chrono::duration_cast<std::chrono::nanoseconds>(1000ms / tps);
			delayBetweenFrames = std::chrono::duration_cast<std::chrono::nanoseconds>(1000ms / fps);

			// setup init timestamps
			time_point gameStart = Clock::now();
			time_point currentTimeTPS = gameStart;
			time_point currentTimeFPS = gameStart;

			// while game is running, do update and draw logic
			while(!shouldShutdown) {
				// process game input
				pollEvents();

				// track current "tick" time points
				time_point newTime = Clock::now();
				auto frameTimeTPS = newTime - currentTimeTPS;
				auto frameTimeFPS = newTime - currentTimeFPS;

				// Anti spiral of death (mostly when debugging stuff, or game paused / minimized)
				if (frameTimeTPS > deadlockBreaker) frameTimeTPS = deadlockBreaker;
				if (frameTimeFPS > deadlockBreaker) frameTimeFPS = deadlockBreaker;

				// ensure we call update as much times per second as requested
				while(frameTimeTPS >= delayBetweenTicks) {
					update(1.0f / static_cast<float>(tps), (newTime - gameStart).count());
					frameTimeTPS -= delayBetweenTicks;
					currentTimeTPS += delayBetweenTicks;
				}

				// we only need to draw a single frame after a update
				// else we're redrawing the same thing without any change
				if (frameTimeFPS >= delayBetweenFrames) {
					draw(1.0f / static_cast<float>(fps));
					currentTimeFPS = newTime;
				}

				// check if we can go sleep to let the cpu rest
				// or if we need to keep going without sleep to keep up
				newTime = Clock::now();
				frameTimeTPS = newTime - currentTimeTPS;
				if (frameTimeTPS >= delayBetweenTicks) {
					runningSlow = true;
					continue;
				}

				runningSlow = false;
				std::this_thread::sleep_for(1ms);
			}
		}

		void Engine::setBreakerTime(const std::chrono::milliseconds& timeBeforeBreaker) {
			deadlockBreaker = timeBeforeBreaker;
		}

		const std::chrono::milliseconds& Engine::getBreakerTime() {
			return deadlockBreaker;
		}

		void Engine::setTPS(unsigned int ticksPerSecond){
			tps = ticksPerSecond;
			delayBetweenFrames = std::chrono::duration_cast<std::chrono::nanoseconds>(1000ms / fps);
		}

		unsigned int Engine::getTPS() {
			return tps;
		}

		void Engine::setFPS(unsigned int framesPerSeond){
			fps = framesPerSeond;
			delayBetweenFrames = std::chrono::duration_cast<std::chrono::nanoseconds>(1000ms / fps);
		}

		unsigned int Engine::getFPS() {
			return fps;
		}

		void Engine::quit() {
			shouldShutdown = true;
		}

		bool Engine::isQuitting() {
			return shouldShutdown;
		}

		bool Engine::isRunningSlow() {
			return runningSlow;
		}
	}
}
