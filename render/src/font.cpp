#include <mainframe/render/font.h>
#include <mainframe/render/color.h>

#include <fmt/format.h>
#include <utf8.h>
#include <iostream>

// https://stackoverflow.com/questions/52086217/outline-fonts-with-true-type-and-opengl-core-3-3

namespace mainframe::render {
	Font::Font(FT_Library& ft, std::string _filename, unsigned int _size, uint32_t _flags) : filename(_filename), size(_size), flags(_flags) {
		if (FT_New_Face(ft, _filename.c_str(), 0, &face) != FT_Err_Ok) {
			throw std::runtime_error(fmt::format("Error: failed to load font: {}", _filename));
		}

		if((flags & TextFlags::STROKE) > 0) {
			if(FT_Stroker_New(ft, &stroker) != FT_Err_Ok) {
				throw std::runtime_error(fmt::format("Error: failed to initialize font stroker: {}", _filename));
			}
		}

		FT_Set_Char_Size(face, 0, size * 64, 72, 72); // DPI = 72
		FT_Select_Charmap(face, FT_ENCODING_UNICODE);

		addChars(" ~!@#$%^&*()_+`1234567890-=QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm|\\<>?,./:;\"'}{][\n\r\t");
	}

	void Font::Release() {
		if(undead) return;

		if (FT_Done_Face(face) != 0) fmt::print(stderr, "Error: failed to clean up font\n");
		if((flags & TextFlags::STROKE) > 0) FT_Stroker_Done(stroker);
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
			loadGlyph(utf8::next(charsIter, chars.end()));
		}
	}

	std::vector<unsigned char> Font::generateGlyphStroke() {
		if((flags & TextFlags::STROKE) == 0) return {};

		FT_Glyph glyphDescStroke;
		if (FT_Get_Glyph(face->glyph, &glyphDescStroke) != FT_Err_Ok) return {};

		double outlineThickness = 2.;
		FT_Stroker_Set(stroker, static_cast<FT_Fixed>(outlineThickness * static_cast<float>(1 << 6)), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0 );

		if(FT_Glyph_StrokeBorder( &glyphDescStroke, stroker, false, true) != FT_Err_Ok) return {};
		if(FT_Glyph_To_Bitmap( &glyphDescStroke, FT_RENDER_MODE_NORMAL, nullptr, true) != FT_Err_Ok) return {};

		FT_BitmapGlyph glyph_bitmap = reinterpret_cast<FT_BitmapGlyph>(glyphDescStroke);
		FT_Bitmap *bitmap_stroke = &glyph_bitmap->bitmap;
		if(bitmap_stroke == nullptr) return {};

		bitmapW = bitmap_stroke->width;
		bitmapR = bitmap_stroke->rows;

		bitmapX = glyph_bitmap->left;
		bitmapY = glyph_bitmap->top;

		face->glyph->advance.x += outlineThickness;
		face->glyph->advance.y += outlineThickness;

		auto buffer = std::vector<unsigned char>(bitmapW * bitmapR * 2, 0); // * 2 -> 2 color channels (red and green)
		for ( unsigned int i = 0; i < bitmapW * bitmapR; ++ i) {
			buffer[i * 2 + 1] = bitmap_stroke->buffer[i]; // + 1 -> 2nd color channel
		}

		FT_Done_Glyph( glyphDescStroke ); // Release
		return buffer;
	}

	std::vector<unsigned char> Font::generateGlyph(std::vector<unsigned char>& buffer) {
		FT_Glyph glyphDescFill;
		if(FT_Get_Glyph(face->glyph, &glyphDescFill) != FT_Err_Ok) return {};
		if(FT_Glyph_To_Bitmap( &glyphDescFill, FT_RENDER_MODE_NORMAL, nullptr, true) != FT_Err_Ok) return {};

		FT_BitmapGlyph glyph_bitmap = reinterpret_cast<FT_BitmapGlyph>(glyphDescFill);
		FT_Bitmap *bitmap_fill = &glyph_bitmap->bitmap;
		if(bitmap_fill == nullptr) return {};

		if((flags & TextFlags::STROKE) == 0) {
			bitmapW = bitmap_fill->width;
			bitmapR = bitmap_fill->rows;

			buffer = std::vector<unsigned char>(bitmapW * bitmapR * 2, 0); // * 2 -> 2 color channels (red and green)
			for ( unsigned int i = 0; i < bitmapW * bitmapR; ++ i)
				buffer[i * 2] = bitmap_fill->buffer[i];      // + 0 -> 1st color channel

		} else {
			unsigned int cx_fill = bitmap_fill->width;
			unsigned int cy_fill = bitmap_fill->rows;

			unsigned int offset_x = (bitmapW - cx_fill) / 2; // offset because the bitmap my be smaller,
			unsigned int offset_y = (bitmapR - cy_fill) / 2; // then the former

			for(unsigned int y = 0; y < cy_fill; ++ y ) {
				for(unsigned int x = 0; x < cx_fill; ++ x ) {
					unsigned int i_source = y * cx_fill + x;
					unsigned int i_target = (y + offset_y) * bitmapW + x + offset_x;

					buffer[i_target * 2] = bitmap_fill->buffer[i_source]; // + 0 -> 1st color channel
				}
			}
		}

		FT_Done_Glyph( glyphDescFill ); // Release
		return buffer;
	}

	Glyph Font::loadGlyph(FT_ULong character) {
		if (hasGlyph(character)) return getGlyph(character);

		FT_UInt charIndx = FT_Get_Char_Index(face, character);
		if (charIndx == 0) return {};

		if (FT_Load_Glyph(face, charIndx, FT_LOAD_NO_BITMAP) != FT_Err_Ok) return {};

		std::vector<unsigned char> buffer = this->generateGlyphStroke();
		buffer = this->generateGlyph(buffer);

		auto& atlasNode = atlas.addSprite(bitmapW, bitmapR);

		// upload glpyh into atlas
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureSubImage2D(
			atlas.glTexture,
			0,
			atlasNode.x,
			atlasNode.y,
			atlasNode.width,
			atlasNode.height,
			GL_RG,
			GL_UNSIGNED_BYTE,
			buffer.data()
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
		auto endIter = utf8::find_invalid(text.begin(), text.end());

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
