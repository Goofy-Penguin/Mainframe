#pragma once

#include <mainframe/ui/elms/label.h>

namespace mainframe {
	namespace ui {
		class Frame : public Label {
			int titleSize = 20;
			int borderSize = 2;
			render::Color borderColor = render::Colors::DarkGray;
			render::Color backColor = render::Colors::Gray;
			bool dragging = false;
			math::Vector2i dragStart;

		public:
			virtual void resizeToContents() override;

			void setTitleSize(int size);
			int getTitleSize();

			void setBorderSize(int size);
			int getBorderSize();

			void setBorderColor(render::Color color);
			const render::Color& getBorderColor();

			void setBackColor(render::Color color);
			const render::Color& getBackColor();

			virtual void draw(render::Stencil& stencil) override;
			virtual void drawAfter(render::Stencil& stencil) override;

			virtual void mouseDown(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods) override;
			virtual void mouseUp(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods) override;
			virtual void mouseMove(const math::Vector2i& mousePos) override;
		};
	}
}