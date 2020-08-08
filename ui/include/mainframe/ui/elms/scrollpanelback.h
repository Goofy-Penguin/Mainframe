#pragma once

#include <mainframe/ui/element.h>
#include <mainframe/ui/elms/scrollbar.h>
#include <mainframe/ui/elms/panel.h>

namespace mainframe {
	namespace ui {
		class ScrollPanelBack : public Element {
			math::Vector2i offset;

		public:
			virtual void draw(render::Stencil& stencil) override;
			virtual void drawAfter(render::Stencil& stencil) override;

			void setOffset(math::Vector2i ofset_);
		};
	}
}