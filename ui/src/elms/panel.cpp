#include <mainframe/ui/elms/panel.h>

namespace mainframe {
	namespace ui {
		void Panel::draw(render::Stencil& stencil) {
			stencil.drawBox({0, 0}, getSize(), render::Colors::Red);
		}
	}
}