#pragma once

#include <game/engine.h>
#include <game/window.h>

namespace tomatris {
	class Game : public mainframe::game::Engine {
		mainframe::game::Window window;

	public:
		Game();
		~Game() = default;

		virtual void update() override;
		virtual void draw() override;
		virtual void init() override;
	};
}