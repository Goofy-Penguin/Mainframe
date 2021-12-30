#include <mainframe/game/engine.h>
#include <mainframe/game/window.h>
#include <fmt/printf.h>
#include <ctime>

using namespace mainframe::game;

class GameTest : public Engine {
	Window& window;
	time_t initTime = 0;

public:
	virtual void init() override {
		initTime = std::time(nullptr);
	}

	virtual void draw(const double alpha) override {
		window.use(); // only needed for multi window

		// draw logic

		window.swapBuffer();
	}

	virtual void quit() override {
		Engine::quit();
		window.close();
	}

	virtual void update(float deltaTime, long long gameTime) override {
		if (window.getShouldClose() || std::time(nullptr) - initTime > 2) {
			quit();
		}
	}

	GameTest(Window& w) : window(w) {

	}
};

int main() {
	Window w;
	if (!w.create(1024, 1024, "mainframe.game.test")) {
		fmt::print("Failed to create window :(\n");
		return -1;
	}

	GameTest e(w);
	e.setTick(75);

	e.init();
	e.run();

	return 0;
}
