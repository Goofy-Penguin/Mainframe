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
			/*double t = 0.0f;
    		const double dt = 0.01f;

			auto lasttick = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - milliseconds(1000);
			auto currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    		double accumulator = 0.0f;

			while (!shouldShutdown) {
				auto msperfps = milliseconds(static_cast<time_t>(1000.0f / targetFPS));
       			milliseconds curtime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

				double frameTime = static_cast<double>(curtime.count() - currentTime.count());
				if ( frameTime > 0.25 ) frameTime = 0.25;

				currentTime = curtime;
       		 	accumulator += frameTime;

				while ( accumulator >= dt ) {
					fixedUpdate(dt);

					t += dt;
					accumulator -= dt;
				}

				if (curtime - lasttick < msperfps){
					std::this_thread::sleep_for(milliseconds(1));
					 continue;
				}

				lasttick += msperfps;
				if (curtime - lasttick > milliseconds(1000)) lasttick = curtime;

				update(0.f);
				draw();
			}*/

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
			}
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
