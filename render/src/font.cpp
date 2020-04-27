#include <mainframe/render/font.h>

#include <freetype-gl.h>
#include <utf8-utils.h>
#include <stdexcept>

namespace mainframe {
	namespace render {
		ftgl::texture_atlas_t*& Font::getAtlas() {
			thread_local ftgl::texture_atlas_t* atlas = nullptr;
			return atlas;
		}

		Font::Font(const std::string& file, float size, bool loadDefaulChars) {
			auto& atlas = getAtlas();

			if (atlas == nullptr) {
				atlas = texture_atlas_new(1024, 1024, 1);
			}

			handle->glHandle = texture_font_new_from_file(atlas, size, file.c_str());
			if (handle->glHandle == nullptr) throw std::runtime_error("error loading font");

			if (loadDefaulChars) {
				addChars(" ~!@#$%^&*()_+`1234567890-=QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm|\\<>?,./:;\"'}{][”“’\n");
			}
		}

		void FontHandle::reset() {
			if (glHandle == nullptr) return;

			texture_font_delete(glHandle);
			glHandle = nullptr;
		}

		FontHandle::~FontHandle() {
			reset();
		}

		void Font::upload() {
			auto& atlas = handle->glHandle->atlas;
			unsigned char* data = (unsigned char*)atlas->data;
			tex.resize({static_cast<int>(atlas->width), static_cast<int>(atlas->height)});
			tex.clear(Colors::White);

			auto& pixels = tex.data();
			for (size_t i = 0; i < atlas->width * atlas->height; i++) {
				pixels[i].a = static_cast<float>(data[i]) / 255;
			}

			tex.upload();
		}

		const ftgl::texture_glyph_t* Font::getGlyph(uint32_t character) const {
			auto& handleG = handle->glHandle->glyphs;
			auto glyphs = reinterpret_cast<ftgl::texture_glyph_t**>(handleG->items);

			for (size_t i = 0; i < handleG->size; i++) {
				auto& curglyph = *glyphs[i];

				if (curglyph.codepoint == character) {
					return &curglyph;
				}
			}

			return nullptr;
		}

		float Font::getKerning(const ftgl::texture_glyph_t* glyph, const ftgl::texture_glyph_t* prevGlyph) const {
			for (size_t i = 0; i < glyph->kerning->size; ++i) {
				auto& kerning = *reinterpret_cast<ftgl::kerning_t**>(glyph->kerning->items)[i];

				if (kerning.codepoint == prevGlyph->codepoint) {
					return kerning.kerning;
				}
			}

			return 0.0f;
		}

		void Font::setSharedHandle(std::shared_ptr<FontHandle>& handle_) {
			handle = handle_;
		}

		std::shared_ptr<FontHandle>& Font::getSharedHandle() {
			return handle;
		}

		float Font::getLineHeight() const {
			return handle->glHandle->height;
		}

		float Font::getLineGap() const {
			return handle->glHandle->linegap;
		}

		math::Vector2 Font::getStringSize(const std::string& text) const {
			math::Vector2 total;
			math::Vector2 pos;

			size_t len = text.size();
			float lineheight = getLineHeight();

			const ftgl::texture_glyph_t* prevGlyph = nullptr;

			for (size_t i = 0; i < text.size(); i += ftgl::utf8_surrogate_len(text.c_str() + i)) {
				uint32_t l = ftgl::utf8_to_utf32(text.c_str() + i);
				if (l == '\n') {
					pos.y += lineheight;
					pos.x = 0;
					prevGlyph = nullptr;
					continue;
				}

				auto glyph = getGlyph(l);
				if (glyph == nullptr) continue;

				if (prevGlyph != nullptr) pos.x += getKerning(glyph, prevGlyph);
				pos.x += glyph->advance_x;

				if (pos.x > total.x) total.x = pos.x;
				if (pos.y + lineheight > total.y) total.y = pos.y + lineheight;

				prevGlyph = glyph;
			}

			return total;
		}

		void Font::addChars(const std::string& chars) {
			if (chars.empty()) return;
			texture_font_load_glyphs(handle->glHandle, chars.c_str());

			upload();
		}

		std::string Font::toUTF8(const std::wstring text) {
			const wchar_t* in = text.c_str();

			std::string out;
			unsigned int codepoint = 0;
			for (in; *in != 0; ++in) {
				if (*in >= 0xd800 && *in <= 0xdbff)
					codepoint = ((*in - 0xd800) << 10) + 0x10000;
				else {
					if (*in >= 0xdc00 && *in <= 0xdfff)
						codepoint |= *in - 0xdc00;
					else
						codepoint = *in;

					if (codepoint <= 0x7f)
						out.append(1, static_cast<char>(codepoint));
					else if (codepoint <= 0x7ff) {
						out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
						out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
					} else if (codepoint <= 0xffff) {
						out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
						out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
						out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
					} else {
						out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
						out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
						out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
						out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
					}

					codepoint = 0;
				}
			}

			return out;
		}

		void Font::addChars(const std::wstring& chars) {
			addChars(toUTF8(chars.c_str()));
		}
	}
}