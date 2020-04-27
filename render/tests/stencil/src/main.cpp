#include <mainframe/game/engine.h>
#include <mainframe/game/window.h>
#include <mainframe/render/stencil.h>
#include <mainframe/render/font.h>
#include <fmt/printf.h>

using namespace mainframe::game;
using namespace mainframe::render;
using namespace mainframe::math;

class GameTest : public Engine {
	Window& window;
	Stencil stencil;
	Font font = Font("fonts/stencil/VeraMono.ttf", 15.0f);
	Texture tex = Texture("textures/stencil/test.png");
	float rot = 0;

public:
	virtual void init() override {
		tex.upload();
	}

	virtual void draw() override {
		window.use(); // only needed for multi window

		stencil.drawBox({0, 0}, stencil.getWindowSize(), Colors::DarkGray);

		stencil.drawTriangle(
			{0,   0}, {0, 0}, Colors::Red,
			{100, 0}, {1, 0}, Colors::Green,
			{0, 100}, {0, 1}, Colors::Blue
		);

		stencil.drawBoxOutlined({350, 100}, {100, 100}, 10, Colors::Gray);
		stencil.drawBox({350, 200}, {100, 100}, Colors::Orange);
		stencil.drawTexture({350, 300}, {100, 100}, tex, Colors::White);
		stencil.drawCircleOutlined({450, 100}, {100, 100}, 60, 10, Colors::Orange);
		stencil.drawCircle({450, 200}, {100, 100}, 60, Colors::Orange);

		stencil.drawText(font, "Hello world", {100, 100}, Colors::Red, Stencil::TextAlignment::Center, Stencil::TextAlignment::Center, rot);

		Vector2 pos = {100, 200};
		stencil.drawText(font, "Hello world", pos, Colors::Red, Stencil::TextAlignment::Center, Stencil::TextAlignment::Center, rot, pos);
		rot += 0.02f;


		stencil.pushOffset({100, 400});

		stencil.pushClipping({0, 0, 200, font.getLineHeight() / 4});
		stencil.drawText(font, "TEXT WITH A GAP IN IT!", {}, Colors::Red);
		stencil.popClipping();

		stencil.pushClipping({0, font.getLineHeight() / 4 * 2, 200, font.getLineHeight() / 4});
		stencil.drawText(font, "TEXT WITH A GAP IN IT!", {}, Colors::Red);
		stencil.popClipping();

		stencil.popOffset();

		stencil.draw();
		window.swapBuffer();
	}

	virtual void tick() override {
		Window::pollEvents();
	}

	virtual void quit() override {
		Engine::quit();
		window.close();
	}

	virtual void update() override {
		if (window.getShouldClose()) {
			quit();
		}
	}

	GameTest(Window& w) : window(w) {
		stencil.setWindowSize(w.getSize());
	}
};

int main() {
	Window w;
	if (!w.create(1024, 1024, "mainframe.game.test")) {
		fmt::print("Failed to create window :(\n");
		return -1;
	}

	GameTest e(w);
	e.setFPS(75);

	e.init();
	e.run();

	return 0;
}