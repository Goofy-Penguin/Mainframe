#include <mainframe/ui/elms/label.h>

using TextAlignment = mainframe::render::Stencil::TextAlignment;

namespace mainframe {
	namespace ui {
		void Label::setColor(const render::Color color_) {
			color = color_;
		}

		void Label::setText(const std::string& text_) {
			text = text_;
		}

		void Label::setFont(render::Font* font_) {
			font = font_;
		}

		void Label::setAlignment(render::Stencil::TextAlignment x, render::Stencil::TextAlignment y) {
			alignmentX = x;
			alignmentY = y;
		}

		void Label::resizeToContents() {
			setSize(font->getStringSize(text));
		}

		const render::Color Label::getColor() {
			return color;
		}

		const std::string& Label::getText() {
			return text;
		}

		const render::Font* Label::getFont() {
			return font;
		}

		render::Stencil::TextAlignment Label::getAlignmentX() {
			return alignmentX;
		}

		render::Stencil::TextAlignment Label::getAlignmentY() {
			return alignmentY;
		}

		void Label::draw(render::Stencil& stencil) {
			auto size = getSize();

			math::Vector2i pos;
			switch (alignmentX) {
				case TextAlignment::Left: break;
				case TextAlignment::Center: pos.x = size.x / 2; break;
				case TextAlignment::Right: pos.x = size.x; break;
			}

			switch (alignmentY) {
				case TextAlignment::Left: break;
				case TextAlignment::Center: pos.y = size.y / 2; break;
				case TextAlignment::Right: pos.y = size.y; break;
			}

			stencil.drawText(*font, text, pos, color, alignmentX, alignmentY);
		}
	}
}