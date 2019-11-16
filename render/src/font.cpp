#include <mainframe/render/font.h>

#include <freetype-gl/freetype-gl.h>
#include <stdexcept>

namespace mainframe {
	namespace render {
		ftgl::texture_atlas_t* Font::atlas = nullptr;

		Font::Font(const std::string& file, float size) {
			if (atlas == nullptr) {
				atlas = texture_atlas_new(1024, 1024, 1);
			}

			handle = texture_font_new_from_file(atlas, size, file.c_str());
			if (handle == nullptr) throw std::runtime_error("error loading font");

			addChars(" ~!@#$%^&*()_+`1234567890-=QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm|\\<>?,./:;\"'}{][”“’\n");
		}

		Font::~Font() {
			if (handle == nullptr) return;
			texture_font_delete(handle);
		}

		void Font::upload() {
			unsigned char* data = (unsigned char*)handle->atlas->data;
			tex.resize({static_cast<int>(handle->atlas->width), static_cast<int>(handle->atlas->height)});
			tex.clear(Colors::White);

			auto& pixels = tex.data();
			for (size_t i = 0; i < handle->atlas->width * handle->atlas->height; i++) {
				pixels[i].a = static_cast<float>(data[i]) / 255;
			}

			tex.upload();
		}

		const ftgl::texture_glyph_t* Font::getGlyph(char character) const {
			auto glyphs = reinterpret_cast<ftgl::texture_glyph_t**>(handle->glyphs->items);

			for (size_t i = 0; i < handle->glyphs->size; i++) {
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

		float Font::getLineHeight() const {
			return handle->height;
		}

		math::Vector2 Font::getStringSize(const std::string& text) const {
			math::Vector2 total;
			math::Vector2 pos;

			size_t len = text.size();
			float lineheight = getLineHeight();

			const ftgl::texture_glyph_t* prevGlyph = nullptr;
			for (auto l : text) {
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
			std::string notfound;
			for (auto letter : chars) {
				if (getGlyph(letter) != nullptr) continue;

				notfound += letter;
			}

			if (notfound.empty()) return;
			texture_font_load_glyphs(handle, notfound.c_str());

			upload();
		}
	}
}