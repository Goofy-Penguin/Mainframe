#include <engine.h>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono;

namespace mainframe {
	namespace game {
		void Engine::tick() {
		}

		void Engine::update() {
		}

		void Engine::draw() {
		}

		void Engine::run() {
			auto lasttick = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - milliseconds(1000);

			while (!shouldShutdown) {
				milliseconds curtime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
				auto msperfps = milliseconds(static_cast<time_t>(1000.0f / targetFPS));

				tick();
				if (shouldShutdown) break;
				if (curtime - lasttick < msperfps) {
					std::this_thread::sleep_for(milliseconds(1));
					continue;
				}

				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				auto framestarttime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
				lasttick += msperfps;
				if (curtime - lasttick > milliseconds(1000)) {
					// we try to make up for ticks that take longer so that we still get the target fps
					// but it would be bad if we turn it into a while(true) at some point, gotta give it a break
					lasttick = curtime;
				}

				update();
				if (shouldShutdown) break;

				draw();

				auto deltatime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - framestarttime;
			}
		}

		void Engine::quit() {
			shouldShutdown = true;
		}

		void Engine::init() {

		}

		void Engine::setFPS(float count) {
			this->targetFPS = count;
		}
	}
}