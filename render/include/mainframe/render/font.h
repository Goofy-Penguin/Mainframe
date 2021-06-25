#pragma once
#include <string>
#include <mainframe/render/texture.h>

namespace ftgl {
	struct texture_atlas_t;
	struct texture_font_t;
	struct texture_glyph_t;
}

namespace mainframe {
	namespace render {
		class FontHandle {
		public:
			ftgl::texture_font_t* glHandle = nullptr;

			void reset();

			FontHandle() = default;
			~FontHandle();
		};

		class Font {
			static ftgl::texture_atlas_t*& getAtlas();
			std::shared_ptr<FontHandle> handle = std::make_shared<FontHandle>();


		public:
			Texture tex;

			Font() = default;
			Font(const std::string& file, float size, bool loadDefaulChars = true);

			math::Vector2 getStringSize(const std::string& text) const;

			void addChars(const std::string& chars);
			void addChars(const std::wstring& chars);
			void upload();

			bool loadFile(const std::string& file, float size, bool loadDefaulChars = true);

			const ftgl::texture_glyph_t* getGlyph(uint32_t character) const;
			float getKerning(const ftgl::texture_glyph_t* glyph, const std::string& utf8Char) const;
			float getLineHeight() const;
			float getLineGap() const;

			std::shared_ptr<FontHandle>& getSharedHandle();
			void setSharedHandle(std::shared_ptr<FontHandle>& handle_);

			static std::string toUTF8(const std::wstring text);
		};
	}
}