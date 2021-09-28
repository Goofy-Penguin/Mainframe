#pragma once

#include <mainframe/ui/elms/frame.h>

namespace mainframe {
	namespace ui {
		class Button : public Frame {
			render::Color hoverColor = render::Color(0.1f, 0.1f, 0.1f, 1);

		public:
			Button();

			mainframe::utils::Event<> onClick;

			void setHoverColor(render::Color color);
			const render::Color& getHoverColor();

			virtual void resizeToContents() override;

			virtual void draw(render::Stencil& stencil) override;
			virtual void drawAfter(render::Stencil& stencil) override;
			virtual void update() override;

			virtual void mouseDown(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods) override;
			virtual void mouseUp(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods) override;
		};
	}
}