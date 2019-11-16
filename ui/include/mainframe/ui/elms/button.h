#pragma once

#include <mainframe/ui/elms/frame.h>
#include <mainframe/utils/event.hpp>

namespace mainframe {
	namespace ui {
		class Button : public Frame {
		public:
			Button();

			mainframe::utils::Event<> onClick;

			virtual void resizeToContents() override;

			virtual void draw(render::Stencil& stencil) override;
			virtual void drawAfter(render::Stencil& stencil) override;
			virtual void update() override;

			virtual void mouseDown(const math::Vector2i& mousePos, unsigned int button, unsigned int mods) override;
			virtual void mouseUp(const math::Vector2i& mousePos, unsigned int button, unsigned int mods) override;
		};
	}
}