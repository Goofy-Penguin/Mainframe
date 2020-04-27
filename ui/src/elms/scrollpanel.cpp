#include <mainframe/ui/elms/scrollpanel.h>

namespace mainframe {
	namespace ui {
		using namespace mainframe::render;

		void ScrollPanel::draw(render::Stencil& stencil) {
			stencil.drawBox({0, 0}, getSize(), render::Color(255, 255, 255, 200));
			stencil.drawBoxOutlined({0, 0}, getSize(), 1, Colors::Black);
			stencil.drawBox(1, getSize() - 2, Color(255, 255, 255, 217));

			stencil.pushOffset(offset);
		}

		void ScrollPanel::drawAfter(render::Stencil& stencil) {
			stencil.popOffset();
		}

		void ScrollPanel::setBars(bool h, bool v) {

		}

		void ScrollPanel::checkBars() {

		}
	}
}