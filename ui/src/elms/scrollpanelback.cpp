#include <mainframe/ui/elms/scrollpanelback.h>

namespace mainframe {
	namespace ui {
		using namespace mainframe::render;

		void ScrollPanelBack::draw(render::Stencil& stencil) {
			stencil.pushOffset(offset);
		}

		void ScrollPanelBack::drawAfter(render::Stencil& stencil) {
			stencil.popOffset();
		}

		void ScrollPanelBack::setOffset(math::Vector2i ofset_) {
			offset = ofset_;
		}
	}
}