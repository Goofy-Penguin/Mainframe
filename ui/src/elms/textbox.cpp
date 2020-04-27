#include <mainframe/ui/elms/textbox.h>

#include <GLFW/glfw3.h>
#include <utf8-utils.h>

#ifdef _MSC_VER
#include <Windows.h>
#endif

namespace mainframe {
	namespace ui {
		const math::Vector2& Textbox::getBorderSize() {
			return sizeBorder;
		}

		const math::Vector2& Textbox::getCharetSize() {
			return sizeCharet;
		}

		void Textbox::setBorderSize(const math::Vector2& size) {
			sizeBorder = size;
		}

		void Textbox::setCharetSize(const math::Vector2& size) {
			sizeCharet = size;
		}

		const render::Color& Textbox::getBorderColor() {
			return colBorder;
		}

		const render::Color& Textbox::getBackgroundColor() {
			return colBackground;
		}

		void Textbox::setBorderColor(const render::Color& col) {
			colBorder = col;
		}

		void Textbox::setBackgroundColor(const render::Color& col) {
			colBackground = col;
		}

		void Textbox::setText(const std::string& text_) {
			Label::setText(text_);
		}

		Textbox::Textbox() {
			setColor(render::Colors::Black);
		}

		const std::string& Textbox::getMask() {
			return maskCharacter;
		}

		void Textbox::setMask(const std::string& mask) {
			maskCharacter = mask;
		}

		void Textbox::draw(render::Stencil& stencil) {
			auto bsize = getBorderSize();

			stencil.drawBoxOutlined({}, getSize(), bsize, getBorderColor());
			stencil.drawBox(bsize, getSize() - bsize * 2, getBackgroundColor());

			auto size = getSize();
			auto start = size / 2;
			auto& font = getFont();
			auto& text = getText();

			auto offset = start - font->getStringSize(text) / 2;

			stencil.drawBox(offset + charetPosXY, getCharetSize(), getBorderColor());
			if (maskCharacter.empty()) {
				stencil.drawText(*font, text, start - font->getStringSize(text) / 2, getColor());
				return;
			}

			std::string masked;
			for (size_t i = 0; i < text.size(); i += ftgl::utf8_surrogate_len(text.c_str() + i)) {
				masked += maskCharacter;
			}

			stencil.drawText(*font, masked, start - font->getStringSize(masked) / 2, getColor());
		}

		void Textbox::keyDown(unsigned int key, ModifierKey mods, bool repeating) {
			if (key == GLFW_KEY_V && (mods & ModifierKey::ctrl) != ModifierKey::none) { combo_paste(); return; } // ctrl V
			if (key == GLFW_KEY_BACKSPACE) { combo_backspace(mods); return; }
			if (key == GLFW_KEY_DELETE) { combo_delete(mods); return; }
			if (key == GLFW_KEY_LEFT) { combo_arrow_left(); return; }
			if (key == GLFW_KEY_RIGHT) { combo_arrow_right(); return; }
			if (key == GLFW_KEY_HOME || key == GLFW_KEY_UP) { combo_home(); return; }
			if (key == GLFW_KEY_END || key == GLFW_KEY_DOWN) { combo_end(); return; }
		}

		void Textbox::keyChar(unsigned int key) {
			auto letter = static_cast<wchar_t>(key);
			std::wstring wstr;
			wstr += letter;

			auto oldtext = getText();
			auto utf8 = mainframe::render::Font::toUTF8(wstr);
			setText(oldtext + utf8);

			charetPos.x++;
			charetPosXY.x += (getFont()->getStringSize(getText()) - getFont()->getStringSize(oldtext)).x;
		}

		void Textbox::combo_paste() {
#ifdef _MSC_VER
			if (!OpenClipboard(nullptr)) return;

			HANDLE pText = GetClipboardData(CF_UNICODETEXT);
			CloseClipboard();

			wchar_t* text = reinterpret_cast<wchar_t*>(GlobalLock(pText));
			size_t tlen = std::wcslen(text);

			auto newtext = getText() + mainframe::render::Font::toUTF8({text, text + tlen});
			charetPosXY.x += (getFont()->getStringSize(newtext) - getFont()->getStringSize(getText())).x;
			setText(newtext);
#endif
		}

		void Textbox::combo_backspace(ModifierKey mods) {
			auto text = getText();
			if (text.empty()) return;

			size_t i = 0;
			while (true) {
				auto next = i + ftgl::utf8_surrogate_len(text.c_str() + i);

				if (text.begin() + next != text.end()) {
					i = next;
					continue;
				}

				text.erase(text.begin() + i, text.end());
				break;
			}

			charetPosXY.x -= (getFont()->getStringSize(getText()) - getFont()->getStringSize(text)).x;
			setText(text);
		}

		void Textbox::combo_delete(ModifierKey mods) {

		}

		void Textbox::combo_home() {

		}

		void Textbox::combo_end() {

		}

		void Textbox::combo_arrow_left() {

		}

		void Textbox::combo_arrow_right() {

		}
	}
}