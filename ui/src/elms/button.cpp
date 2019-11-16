#include <mainframe/ui/elms/button.h>

using TextAlignment = mainframe::render::Stencil::TextAlignment;

namespace mainframe {
	namespace ui {
		Button::Button() {
			setAlignment(TextAlignment::Center, TextAlignment::Center);
		}

		void Button::drawAfter(render::Stencil& stencil) {}
		void Button::draw(render::Stencil& stencil) {
			auto bsize = getBorderSize();

			stencil.drawBoxOutlined({}, getSize(), static_cast<float>(bsize), getBorderColor());
			stencil.drawBox(static_cast<float>(bsize), getSize() - bsize * 2, getBackColor());

			auto size = getSize();

			math::Vector2i pos(bsize);
			switch (getAlignmentX()) {
				case TextAlignment::Left: break;
				case TextAlignment::Center: pos.x = size.x / 2; break;
				case TextAlignment::Right: pos.x = size.x - bsize * 2; break;
			}

			switch (getAlignmentY()) {
				case TextAlignment::Left: break;
				case TextAlignment::Center: pos.y = size.y / 2; break;
				case TextAlignment::Right: pos.y = size.y - bsize * 2; break;
			}

			stencil.drawText(*getFont(), getText(), pos, getColor(), getAlignmentX(), getAlignmentY());
		}

		void Button::resizeToContents() {
			Label::resizeToContents();
			setSize(getSize() + getBorderSize() * 2);
		}

		void Button::update() {

		}

		void Button::mouseDown(const math::Vector2i& mousePos, unsigned int button, unsigned int mods) {

		}

		void Button::mouseUp(const math::Vector2i& mousePos, unsigned int button, unsigned int mods) {
			onClick();
		}
	}
}