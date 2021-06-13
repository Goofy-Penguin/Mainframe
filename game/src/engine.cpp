#include <mainframe/game/engine.h>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono;

namespace mainframe {
	namespace game {
		void Engine::update(float deltaTime) { }
		void Engine::draw() { }

		void Engine::run() {
			auto currentTime = high_resolution_clock::now().time_since_epoch();

			float t = 0.f;
			const float dt = 0.01f;
			float accumulator = 0.f;

			while (!shouldShutdown) {
				auto newTime = high_resolution_clock::now().time_since_epoch();
       			auto frameTime = static_cast<float>(duration_cast<milliseconds>(newTime - currentTime).count());
				if ( frameTime > timeStep ) frameTime = timeStep;

    			currentTime = newTime;
    			accumulator += frameTime;

				while ( accumulator >= dt ) {
       				update(dt);

					t += dt;
					accumulator -= dt;
				}

				if (shouldShutdown) break;
				draw();

				std::this_thread::sleep_for(milliseconds(1));
			}
		}

		void Engine::setTimestep(float time){
			timeStep = time;
		}

		void Engine::quit() {
			shouldShutdown = true;
		}

		bool Engine::isQuitting() {
			return shouldShutdown;
		}

		void Engine::init() {

		}
	}
}
