#pragma once

#include <mainframe/ui/element.h>

namespace mainframe {
	namespace ui {
		class Panel : public Element {
		public:
			virtual void draw(render::Stencil& stencil) override;
		};
	}
}