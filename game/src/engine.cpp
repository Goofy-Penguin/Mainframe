#include <mainframe/game/engine.h>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono;

namespace mainframe {
	namespace game {
		void Engine::update(float deltaTime) { }
		void Engine::fixedUpdate(float deltaTime) { }
		void Engine::draw() { }

		void Engine::run() {
			float t = 0.0f;
			const float physicsTicks = 20;
			const float dt = 1000.0 / physicsTicks;
			auto engineStart = system_clock::now().time_since_epoch();

			auto currentTime = 0.0;
			auto lastFrame = currentTime;
			float accumulator = 0.0f;

			while (!shouldShutdown) {
				auto msperfps = 1000.0f / targetFPS;

       			auto newTime = static_cast<float>(duration_cast<milliseconds>(system_clock::now().time_since_epoch() - engineStart).count());
				auto frameTime = newTime - currentTime;
				if (frameTime > dt * 25) frameTime = dt * 25;

       		 	accumulator += frameTime;
				while (accumulator >= dt) {
					fixedUpdate(1.0 / physicsTicks);

					t += dt;
					accumulator -= dt;
				}

				if (currentTime - lastFrame < msperfps){
					std::this_thread::sleep_for(milliseconds(1));
					currentTime = newTime;
					continue;
				}

				update((currentTime - lastFrame) / msperfps);
				draw();

				currentTime = newTime;
				lastFrame += msperfps;

				// skip to future incase we're getting behind too much
				// TODO: bool to allow it or not, for games we'd not want this
				//if (currentTime - lastFrame > 1000) {
				//	lastFrame = currentTime;
				//}
			}

			/*
			auto lasttick = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - milliseconds(1000);

			while (!shouldShutdown) {
				milliseconds curtime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
				auto msperfps = milliseconds(static_cast<time_t>(1000.0f / targetFPS));

				update(0.f);
				if (shouldShutdown) break;
				if (curtime - lasttick < msperfps) {
					std::this_thread::sleep_for(milliseconds(1));
					continue;
				}

				auto framestarttime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
				lasttick += msperfps;
				if (curtime - lasttick > milliseconds(1000)) {
					// we try to make up for ticks that take longer so that we still get the target fps
					// but it would be bad if we turn it into a while(true) at some point, gotta give it a break
					lasttick = curtime;
				}

				fixedUpdate(1.f / 60.f);
				if (shouldShutdown) break;

				draw();
			}*/
		}

		void Engine::quit() {
			shouldShutdown = true;
		}

		bool Engine::isQuitting() {
			return shouldShutdown;
		}

		void Engine::init() {

		}

		void Engine::setFPS(float count) {
			this->targetFPS = count;
		}
	}
}
