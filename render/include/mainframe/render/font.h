#pragma once
#include <string>
#include <mainframe/render/texture.h>

namespace ftgl {
	class texture_atlas_t;
	class texture_font_t;
	class texture_glyph_t;
}

namespace mainframe {
	namespace render {
		class Font {
			static ftgl::texture_atlas_t* atlas;
			ftgl::texture_font_t* handle = nullptr;

			void upload();

		public:
			Texture tex;

			Font() = default;
			Font(const std::string& file, float size);
			~Font();

			math::Vector2 getStringSize(const std::string& text) const;

			void addChars(const std::string& chars);

			const ftgl::texture_glyph_t* getGlyph(char character) const;
			float getKerning(const ftgl::texture_glyph_t* glyph, const ftgl::texture_glyph_t* prevGlyph) const;
			float getLineHeight() const;
		};
	}
}