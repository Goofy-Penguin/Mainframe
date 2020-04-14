#include <mainframe/ui/elms/frame.h>
#include <cmath>

using TextAlignment = mainframe::render::Stencil::TextAlignment;

namespace mainframe {
	namespace ui {
		void Frame::resizeToContents() {
			titleSize = static_cast<int>(std::ceil(getFont()->getStringSize(getText()).y));
		}

		void Frame::setTitleSize(int size) {
			titleSize = size;
		}

		int Frame::getTitleSize() {
			return titleSize;
		}

		void Frame::setBorderSize(int size) {
			borderSize = size;
		}

		int Frame::getBorderSize() {
			return borderSize;
		}

		void Frame::setBorderColor(render::Color color) {
			borderColor = color;
		}

		const render::Color& Frame::getBorderColor() {
			return borderColor;
		}

		void Frame::setBackColor(render::Color color) {
			backColor = color;
		}

		const render::Color& Frame::getBackColor() {
			return backColor;
		}

		void Frame::mouseMove(const math::Vector2i& mousePos) {
			if (!dragging) return;

			auto p = getParent<ElementContainer>();
			setPos((getPos() + (mousePos - dragStart)).clamp(p->getPos(), p->getPos() + p->getSize() - getSize()));
		}

		void Frame::mouseDown(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods) {
			if (mousePos.y >= titleSize) return;
			dragging = true;
			dragStart = mousePos;
		}

		void Frame::mouseUp(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods) {
			dragging = false;
		}

		void Frame::draw(render::Stencil& stencil) {
			auto& size = getSize();
			math::Vector2i titleBar = {size.x, titleSize};
			stencil.drawBox({}, titleBar, borderColor);

			auto contentStart = math::Vector2i(0, titleSize);
			if (borderSize == 0) {
				stencil.drawBox(contentStart, size - contentStart, backColor);
			} else {
				stencil.drawBoxOutlined(contentStart, size - contentStart, static_cast<float>(borderSize), borderColor);
				stencil.drawBox(contentStart + math::Vector2i(borderSize, borderSize), size - borderSize * 2 - contentStart, backColor);
			}


			math::Vector2i pos(borderSize);
			switch (getAlignmentX()) {
				case TextAlignment::Left: break;
				case TextAlignment::Center: pos.x = size.x / 2; break;
				case TextAlignment::Right: pos.x = size.x; break;
			}

			switch (getAlignmentY()) {
				case TextAlignment::Left: break;
				case TextAlignment::Center: pos.y = titleSize / 2; break;
				case TextAlignment::Right: pos.y = titleSize; break;
			}

			stencil.drawText(*getFont(), getText(), pos, getColor(), getAlignmentX(), getAlignmentY());

			stencil.pushOffset(contentStart + borderSize);
		}

		void Frame::drawAfter(render::Stencil& stencil) {
			stencil.popOffset();
		}
	}
}