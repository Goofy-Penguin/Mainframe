#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <mainframe/render/font.h>

namespace mainframe {
	namespace render {
		// Not threadsafe, make one engine per thread
		class TextEngine {
			FT_Library ft;
			std::vector<std::unique_ptr<Font>> faces;

		public:
			TextEngine();
			TextEngine(const TextEngine& other) = delete;
			TextEngine(TextEngine&& other) = delete;
			
			TextEngine& operator=(const TextEngine& other) = delete;
			TextEngine& operator=(TextEngine&& other) = delete;
			
			~TextEngine();

			Font& loadFont(std::string filename, unsigned int size);
		};
	}
}
