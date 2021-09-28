#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <vector>
#include <mainframe/render/atlas.h>
#include <mainframe/render/textglyph.h>

namespace mainframe {
	namespace render {
		class Font {
		// TODO
		public:
			std::string filename;
			FT_Face face;
			Atlas atlas;
			std::vector<Glyph> glyphs;
			bool undead = false; // needs to be movable, and moved from state is undead.

		public:
			Font(FT_Library& ft, std::string filename, unsigned int size);
			Font(const Font& other) = delete;
			Font(Font&& other) = delete;

			Font& operator=(const Font& other) = delete;
			Font& operator=(Font&& other) = delete;

			void Release();

			~Font();

			// distance between two lines of text that are under eachother
			float getLineHeight() const;

			// "global glyph height", should be the highest any character gets
			float textHeight() const;

			void addChars(std::string chars);

			float getKerning(const Glyph& left, const Glyph& right) const;

			Glyph loadGlyph(FT_ULong character);

			const Glyph& getGlyph(uint32_t codepoint) const;

			math::Vector2 getStringSize(const std::string& text) const;

			static std::string toUTF8(const std::wstring text);
		};
	}
}