#pragma once

#include <mainframe/ui/element.h>
#include <mainframe/render/font.h>
#include <mainframe/render/color.h>

namespace mainframe {
	namespace ui {
		class Label : public Element {
			std::shared_ptr<render::Font> font;
			render::Color color = render::Colors::White;
			std::string text;
			render::Stencil::TextAlignment alignmentX = render::Stencil::TextAlignment::Left;
			render::Stencil::TextAlignment alignmentY = render::Stencil::TextAlignment::Left;

		public:
			void setColor(const render::Color color_);
			void setText(const std::string& text_);
			void setFont(const std::shared_ptr<render::Font>& font_);
			void setAlignment(render::Stencil::TextAlignment x, render::Stencil::TextAlignment y);
			void setAlignmentX(render::Stencil::TextAlignment x);
			void setAlignmentY(render::Stencil::TextAlignment y);

			const render::Color getColor();
			const std::string& getText();
			const std::shared_ptr<render::Font>& getFont( );
			render::Stencil::TextAlignment getAlignmentX();
			render::Stencil::TextAlignment getAlignmentY();

			virtual void resizeToContents();

			virtual void draw(render::Stencil& stencil) override;
		};
	}
}