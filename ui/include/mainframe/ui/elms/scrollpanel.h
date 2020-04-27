#pragma once

#include <mainframe/ui/element.h>

namespace mainframe {
	namespace ui {
		class ScrollPanel : public Element {
			math::Vector2i offset;

		public:
			virtual void draw(render::Stencil& stencil) override;
			virtual void drawAfter(render::Stencil& stencil) override;

			void setBars(bool h, bool v = false);
			void checkBars();
		};
	}
}