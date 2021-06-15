#include <mainframe/game/engine.h>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono;

namespace mainframe {
	namespace game {
		void Engine::init() { }
		void Engine::update(float deltaTime) { }
		void Engine::draw() { }

		void Engine::run() {
			auto prevTime = high_resolution_clock::now().time_since_epoch();

			float timeSinceLastRender = 0;
			float timeStep = 1.f / static_cast<float>(tick);

			while (!shouldShutdown) {

				auto currTime = high_resolution_clock::now().time_since_epoch();
        		float durationOfLastFrame = static_cast<float>(duration_cast<milliseconds>(currTime - prevTime).count()) / 1000.f;
				prevTime = currTime;

				timeSinceLastRender += durationOfLastFrame;

				while (timeSinceLastRender >= timeStep) {
           			update(timeStep);
					timeSinceLastRender -= timeStep;
				}

				if (shouldShutdown) break;
				draw();

				std::this_thread::sleep_for(milliseconds(1000 / tick));
			}
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
