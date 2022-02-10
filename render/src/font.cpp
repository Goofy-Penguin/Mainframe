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

			FT_Set_Char_Size(face, 0, size * 64, 72, 72); // DPI = 72
			FT_Select_Charmap(face, FT_ENCODING_UNICODE);

			addChars(" ~!@#$%^&*()_+`1234567890-=QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm|\\<>?,./:;\"'}{][\n\r\t");
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
			return static_cast<float>(face->size->metrics.height >> 6);
		}

		float Font::lineGap() const {
			auto& metrics = face->size->metrics;
			return static_cast<float>(
				(metrics.height >> 6) -
				(metrics.ascender >> 6) +
				(metrics.descender >> 6)
			);
		}

		float Font::getKerning(const Glyph& left, const Glyph& right) const {
			FT_Vector kerning;

			FT_Get_Kerning(face, left.glyphIndex, right.glyphIndex, FT_KERNING_UNFITTED, &kerning);
			return static_cast<float>(kerning.x >> 6);
		}


		void Font::addChars(std::string chars) {
			auto charsIter = chars.begin();
			while(charsIter < chars.end()) {
				auto chr = utf8::next(charsIter, chars.end());
				loadGlyph(chr);
			}
		}

		Glyph Font::loadGlyph(FT_ULong character) {
			if (hasGlyph(character)) return getGlyph(character);

			FT_UInt charIndx = FT_Get_Char_Index(face, character);
			if (charIndx == 0) {
				//throw std::runtime_error(fmt::format("face {} does not have character {}", filename, character));
				return {};
			}

			if (FT_Load_Glyph(face, charIndx, FT_LOAD_DEFAULT ) != FT_Err_Ok) {
				//throw std::runtime_error(fmt::format("Error: failed to load char: {}\n", character));
				return {};
			}

			if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL ) != FT_Err_Ok) {
				//throw std::runtime_error(fmt::format("Error: failed to load char: {}\n", character));
				return {};
			}

			auto& atlasNode = atlas.addSprite(face->glyph->bitmap.width, face->glyph->bitmap.rows);

			// upload glpyh into atlas
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTextureSubImage2D(
				atlas.glTexture,
				0,
				atlasNode.x,
				atlasNode.y,
				atlasNode.width,
				atlasNode.height,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);

			Glyph glyph {
				character,
				charIndx,
				{
					static_cast<float>(face->glyph->metrics.horiBearingX) / 64.f,
					static_cast<float>(face->glyph->metrics.horiBearingY) / 64.f
				},
				{
					static_cast<float>(face->glyph->advance.x >> 6),
					static_cast<float>(face->glyph->advance.y >> 6)
				},
				{
					atlasNode.x / static_cast<float>(atlas.size),
					atlasNode.y / static_cast<float>(atlas.size)
				},
				{
					(atlasNode.x + atlasNode.width) / static_cast<float>(atlas.size),
					(atlasNode.y + atlasNode.height) / static_cast<float>(atlas.size)
				},
				{
					static_cast<float>(face->glyph->metrics.width) / 64.f,
					static_cast<float>(face->glyph->metrics.height) / 64.f
				},
			};

			glyphs.push_back(glyph);
			return glyph;
		}

		bool Font::hasGlyph(uint32_t codepoint) const {
			return std::find_if(glyphs.begin(), glyphs.end(), [codepoint](auto glyph) {
				return glyph.codepoint == codepoint;
			}) != glyphs.end();
		}

		const Glyph& Font::getGlyph(uint32_t codepoint) const {
			return *std::find_if(glyphs.begin(), glyphs.end(), [codepoint](auto glyph) { return glyph.codepoint == codepoint; });
		}

		math::Vector2 Font::getStringSize(const std::string& text) const {
			const float lineheight = getLineHeight();
			if (text.empty()) return {0, lineheight};

			math::Vector2 total;
			math::Vector2 pos;


			const Glyph* prevGlyph = nullptr;

			uint32_t point = 0;
			auto beginIter = text.begin();
			auto endIter = text.end();
			while (beginIter != endIter) {
				point = utf8::next(beginIter, endIter);
				if (point == '\n') {
					pos.y += lineheight;
					pos.x = 0;
					prevGlyph = nullptr;
					continue;
				}

				if (!hasGlyph(point)) continue;
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

		size_t Font::getByteCount(const std::string& text, int characterPosition) {
			if (characterPosition == 0) return 0;

			int count = 0;
			auto beginIter = text.begin();
			auto endIter = text.end();
			while (beginIter != endIter) {
				utf8::next(beginIter, endIter);

				count++;
				if (characterPosition == count) {
					return std::distance(text.begin(), beginIter);
				}
			}

			return text.size();
		}

		size_t Font::getCharacterCount(const std::string& text) {
			size_t count = 0;
			auto beginIter = text.begin();
			auto endIter = text.end();
			while (beginIter != endIter) {
				utf8::next(beginIter, endIter);
				count++;
			}

			return count;
		}

		std::string Font::toUTF8(const std::wstring text) {
			std::string result;
			utf8::utf16to8(text.begin(), text.end(), std::back_inserter(result));
			return result;
		}
	}
}
