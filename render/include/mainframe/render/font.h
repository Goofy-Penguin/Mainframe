#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#include <mainframe/render/atlas.h>
#include <mainframe/render/textglyph.h>

#include <string>
#include <vector>

namespace mainframe::render {

	namespace TextFlags {
		const uint32_t NONE = 0;
		const uint32_t STROKE = 1 << 1;
	};

	class Font {
		public:
			std::string filename;
			FT_Face face;
			FT_Stroker stroker;
			Atlas atlas;
			std::vector<Glyph> glyphs;
			uint32_t flags = 0;
			unsigned int size = 0;
			bool undead = false; // needs to be movable, and moved from state is undead.

			/// SIZE ----
			unsigned int bitmapR = -1;
			unsigned int bitmapW = -1;
			FT_Int bitmapX = -1;
			FT_Int bitmapY = -1;

		public:
			Font(FT_Library& ft, std::string filename, unsigned int size, uint32_t flags);
			Font(const Font& other) = delete;
			Font(Font&& other) = delete;

			Font& operator=(const Font& other) = delete;
			Font& operator=(Font&& other) = delete;

			void Release();

			~Font();

			float getLineHeight() const;

			float lineGap() const;

			void addChars(std::string chars);

			float getKerning(const Glyph& left, const Glyph& right) const;

			std::vector<unsigned char> generateGlyphStroke();
			std::vector<unsigned char> generateGlyph(std::vector<unsigned char>& buffer);
			Glyph loadGlyph(FT_ULong character);

			bool hasGlyph(uint32_t codepoint) const;
			const Glyph& getGlyph(uint32_t codepoint) const;

			math::Vector2 getStringSize(const std::string& text) const;
			static size_t getByteCount(const std::string& text, int characterPosition);
			static size_t getCharacterCount(const std::string& text);

			static std::string toUTF8(const std::wstring text);
	};

}
