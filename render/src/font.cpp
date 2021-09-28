#include <mainframe/render/font.h>
#include <mainframe/render/color.h>

#include <fmt/format.h>
#include <utf8.h>
#include <iostream>

namespace mainframe {
	namespace render {
		Font::Font(FT_Library& ft, std::string _filename, unsigned int _size) : filename(_filename), size(_size) {
			if (FT_New_Face(ft, _filename.c_str(), 0, &face) != 0) {
				throw std::runtime_error(fmt::format("Error: failed to load font: {}", _filename));
			}

			FT_Set_Pixel_Sizes(face, 0, size);

			addChars(" ~!@#$%^&*()_+`1234567890-=QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm|\\<>?,./:;\"'}{][\n");
		}

		void Font::Release() {
			if(!undead) {
				if (FT_Done_Face(face) != 0) {
					fmt::print(stderr, "Error: failed to clean up font\n");
				}
			}
		}

		Font::~Font() {
			Release();
		}

		float Font::getLineHeight() const {
			return face->size->metrics.height / 64.f;
		}

		float Font::textHeight() const {
			auto& metrics = face->size->metrics;
			return (metrics.ascender + metrics.descender) / 64.f;
		}

		float Font::getKerning(const Glyph& left, const Glyph& right) const {
			FT_Vector kerning;

			FT_Get_Kerning(face, left.glyphIndex, right.glyphIndex, FT_KERNING_DEFAULT, &kerning);
			return kerning.x / 64.f;
		}


		void Font::addChars(std::string chars) {
			auto charsIter = chars.begin();
			while(charsIter < chars.end()) {
				auto chr = utf8::next(charsIter, chars.end());
				loadGlyph(chr);
			}
		}

		Glyph Font::loadGlyph(FT_ULong character) {
			// if (!FT_Get_Char_Index(face, character)) {
			// 	throw std::runtime_error(fmt::format("face {} does not have character {}", filename, character));
			// }

			FT_Set_Pixel_Sizes(face, 0, size);
			if (!FT_Load_Char(face, character, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT)) {
				//ssssssssshhhh
				//throw std::runtime_error(fmt::format("Error: failed to load char: {}\n", character));
			}

			auto& atlasNode = atlas.addSprite(face->glyph->bitmap.width, face->glyph->bitmap.rows);

			// convert glyph pixels to color
			std::vector<mainframe::render::Color> pixels(atlasNode.width * atlasNode.height);
			for (size_t i = 0; i < pixels.size(); i++) {
				auto byteval = face->glyph->bitmap.buffer[i];
				pixels[i] = {1, 1, 1, static_cast<float>(byteval) / 255.0f};
			}

			// upload glpyh into atlas
			glTextureSubImage2D(
				atlas.glTexture,
				0,
				atlasNode.x,
				atlasNode.y,
				atlasNode.width,
				atlasNode.height,
				GL_RGBA,
				GL_FLOAT,
				pixels.data()
			);

			Glyph glyph {
				character,
				face->glyph->glyph_index,
				{ face->glyph->metrics.horiBearingX / 64.0f, face->glyph->metrics.horiBearingY / 64.0f },
				{ face->glyph->advance.x / 64.0f, face->glyph->advance.y / 64.0f },
				{ atlasNode.x / static_cast<float>(atlas.size), atlasNode.y / static_cast<float>(atlas.size) },
				{ (atlasNode.x + atlasNode.width) / static_cast<float>(atlas.size), (atlasNode.y + atlasNode.height) / static_cast<float>(atlas.size) },
				{ face->glyph->metrics.width / 64.0f, face->glyph->metrics.height / 64.0f },
			};

			glyphs.push_back(glyph);

			return glyph;
		}

		const Glyph& Font::getGlyph(uint32_t codepoint) const {
			return *std::find_if(glyphs.begin(), glyphs.end(), [codepoint](auto glyph) { return glyph.codepoint == codepoint; });
		}

		math::Vector2 Font::getStringSize(const std::string& text) const {
			math::Vector2 total;
			math::Vector2 pos;

			const float lineheight = getLineHeight();

			const Glyph* prevGlyph = nullptr;

			auto textIter = text.begin();
			auto point = utf8::next(textIter, text.end());
			for (; textIter < text.end(); point = utf8::next(textIter, text.end())) {
				if (point == '\n') {
					pos.y += lineheight;
					pos.x = 0;
					prevGlyph = nullptr;
					continue;
				}

				const auto& glyph = getGlyph(point);
				auto maxh = std::max(static_cast<float>(glyph.size.y), lineheight);

				if (prevGlyph != nullptr) pos.x += getKerning(glyph, *prevGlyph);
				pos.x += glyph.advance.x;

				if (pos.x > total.x) total.x = pos.x;
				if (pos.y + maxh > total.y) total.y = pos.y + maxh;

				prevGlyph = &glyph;
			}

			return total;
		}

		std::string Font::toUTF8(const std::wstring text) {
			std::string result;
			utf8::utf16to8(text.begin(), text.end(), std::back_inserter(result));
			return result;
		}
	}
}