#include <mainframe/game/engine.h>
#include <mainframe/game/window.h>
#include <mainframe/render/stencil.h>
#include <mainframe/render/textengine.h>
#include <mainframe/ui/scene.h>
#include <mainframe/ui/elms/panel.h>
#include <mainframe/ui/elms/label.h>
#include <mainframe/ui/elms/frame.h>
#include <mainframe/ui/elms/button.h>
#include <mainframe/ui/elms/image.h>
#include <fmt/printf.h>
#include <stdexcept>
#include <iostream>

using namespace mainframe::game;
using namespace mainframe::render;
using namespace mainframe::math;
using namespace mainframe::ui;

class GameTest : public Engine {
	Window& window;
	Stencil stencil;
	std::shared_ptr<Scene> scene = Scene::create();
	TextEngine textEngine;
	Font& font = textEngine.loadFont("fonts/simplemenu/VeraMono.ttf", 15);
	Font& fontSmall = textEngine.loadFont("fonts/simplemenu/VeraMono.ttf", 13);
	Texture tex = Texture("textures/simplemenu/test.png");

public:
	Texture tmp;
	virtual void init() override {
		scene->setWindow(window);

		auto frame = scene->createChild<Frame>();
		frame->setSize(window.getSize() /  2);
		frame->setPos(window.getSize() / 2 - frame->getSize() / 2);
		frame->setFont(&fontSmall);
		frame->setText("Test frame");
		frame->resizeToContents();

		auto lbl = frame->createChild<Label>();
		lbl->setPos({20, 20});
		lbl->setFont(&font);
		lbl->setText("Some basic menu elements");
		lbl->resizeToContents();

		auto btn = frame->createChild<Button>();
		btn->setPos(lbl->getPos() + Vector2i(0, lbl->getSize().y + 5));
		btn->setFont(&font);
		btn->setText("Press me!");
		btn->resizeToContents();
		btn->setSize(btn->getSize() * 2);
		btn->onClick += [btn]() {
			btn->setText(":0");
			btn->setBorderColor(Colors::Red);
		};

		auto img = frame->createChild<Image>();
		img->setPos(btn->getPos() + Vector2i(0, btn->getSize().y + 5));
		img->setSize(tex.getSize());
		img->setImage(tex);

		std::vector<float> data(font.atlas.size * font.atlas.size);
		glGetTextureImage(font.atlas.glTexture, 0, GL_RED, GL_FLOAT, data.size() * sizeof(float), data.data());
		tmp = {Vector2i(font.atlas.size, font.atlas.size)};
		for (int x = 0; x < font.atlas.size; x++) {
			for (int y = 0; y < font.atlas.size; y++) {
				tmp.setPixel({x, y}, data[y * font.atlas.size + x]);
			}
		}
		tmp.upload();
	}

	virtual void draw() override {
		auto wsize = Vector2(stencil.getWindowSize());

		stencil.drawPolygon({
			{
				{{0, 0},		{0, 0}, {0.2f, 0.0f, 0.0f}},
				{{wsize.x, 0},	{1, 0}, {0.0f, 0.2f, 0.0f}},
				{{0, wsize.y},	{0, 1}, {0.0f, 0.0f, 0.2f}},
				{wsize,			{1, 1}, {0.2f, 0.0f, 0.2f}}
			},
			{
				0, 1, 2,
				1, 2, 3
			}
		});

		scene->draw(stencil);


		/// //////////////////
		stencil.drawBox(10, {600, 30}, Colors::Gray);

		std::string text = " ~!@#$%^&*()_+`1234567890-=QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm|\\<>?,./:;\"'}{][\n";
		int x = 20;
		for (auto letter : text) {
			auto glyph = font.getGlyph(letter);
			stencil.drawTexture({x, 20}, glyph.size, tmp, Colors::White, glyph.textureTopLeft, glyph.textureBottomRight);

			x += glyph.advance.x;
		}

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
		scene->update();

		if (window.getShouldClose()) {
			quit();
		}
	}

	GameTest(Window& w) : window(w) {
		stencil.setWindowSize(w.getSize());
	}
};

int main(int argc, char* argv[]) {
	Window w;
	if (!w.create(1024, 1024, "mainframe.ui.simple")) {
		fmt::print("Failed to create window\n");
		return -1;
	}

	try {
		GameTest e(w);
		e.setFPS(75);

		e.init();
		e.run();
	} catch (std::runtime_error& err) {
		std::cerr << err.what();
	}

	return 0;
}