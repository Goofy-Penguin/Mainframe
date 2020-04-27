#pragma once

#include <mainframe/ui/elms/button.h>

namespace mainframe {
	namespace ui {
		class Textbox : public Label {
			render::Color colBackground = render::Colors::White;
			render::Color colBorder = render::Colors::Gray;

			math::Vector2 sizeBorder {2, 2};
			math::Vector2 sizeCharet {2, 8};

			math::Vector2 charetPos {0, 0};
			math::Vector2 charetPosXY {0, 0};

			std::string maskCharacter;

			void combo_paste();
			void combo_backspace(ModifierKey mods);
			void combo_delete(ModifierKey mods);
			void combo_home();
			void combo_end();
			void combo_arrow_left();
			void combo_arrow_right();

		public:
			Textbox();

			const math::Vector2& getBorderSize();
			const math::Vector2& getCharetSize();
			const render::Color& getBorderColor();
			const render::Color& getBackgroundColor();

			void setBorderSize(const math::Vector2& size);
			void setCharetSize(const math::Vector2& size);
			void setBorderColor(const render::Color& col);
			void setBackgroundColor(const render::Color& col);

			const std::string& getMask();
			void setMask(const std::string& mask);

			virtual void setText(const std::string& text_);
			virtual void draw(render::Stencil& stencil) override;
			virtual void keyDown(unsigned int key, ModifierKey mods, bool repeating) override;
			virtual void keyChar(unsigned int key) override;
		};
	}
}