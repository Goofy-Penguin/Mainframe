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
		void Engine::update(float deltaTime) {}
		void Engine::updateEnd(float deltaTime) {}

		void Engine::draw(const double alpha) {}
		void Engine::run() {
			auto constexpr dt = std::chrono::duration<long long, std::ratio<1, 60>>{1}; // TODO: PASS TICK TO RATIO
			using duration = decltype(Clock::duration{} + dt);
			using time_point = std::chrono::time_point<Clock, duration>;

			time_point t{};
			time_point currentTime = Clock::now();
			duration accumulator = 0s;

			while(!shouldShutdown) {
				time_point newTime = Clock::now();
				auto frameTime = newTime - currentTime;
				if (frameTime > 250ms) frameTime = 250ms; // Anti spiral of death (mostly when debugging stuff, or game paused / minimized) // TODO: PASS Maxframe

				currentTime = newTime;
    			accumulator += frameTime;

				pollEvents();

				while (accumulator >= dt) {
					update(1.f / 60.f); // TODO: Get DT

					t += dt;
        			accumulator -= dt;
				}

				const double alpha = std::chrono::duration<double>{accumulator} / dt;
				draw(alpha);
			}
		}

		unsigned int Engine::getTick(){
			return tick;
		}

		void Engine::setTick(unsigned int t){
			tick = t;
		}

		void Engine::quit() {
			shouldShutdown = true;
		}

		bool Engine::isQuitting() {
			return shouldShutdown;
		}
	}
}
