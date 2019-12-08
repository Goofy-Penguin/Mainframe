#pragma once

#include <mainframe/ui/elms/frame.h>
#include <mainframe/utils/event.hpp>

namespace mainframe {
	namespace ui {
		class Button : public Frame {
			render::Color hoverColor = render::Colors::Gray + 0.1f;

		public:
			Button();

			mainframe::utils::Event<> onClick;

			void setHoverColor(render::Color color);
			const render::Color& getHoverColor();

			virtual void resizeToContents() override;

			virtual void draw(render::Stencil& stencil) override;
			virtual void drawAfter(render::Stencil& stencil) override;
			virtual void update() override;

			virtual void mouseDown(const math::Vector2i& mousePos, unsigned int button, unsigned int mods) override;
			virtual void mouseUp(const math::Vector2i& mousePos, unsigned int button, unsigned int mods) override;
		};
	}
}