#include <mainframe/render/textengine.h>

#include <stdexcept>
#include <fmt/format.h>

namespace mainframe {
	namespace render {
		TextEngine::TextEngine() {
			if (FT_Init_FreeType(&ft) != 0) {
				throw std::runtime_error("Error: failed to initialize freetype");
			}
		}

		TextEngine::~TextEngine() {
			// Clear faces now so they can clean up before the freetype library
			// is cleaned up.
			faces.clear();

			if (FT_Done_FreeType(ft) != 0) {
				fmt::print(stderr, "Error: failed to clean up freetype\n");
			}
		}

		Font& TextEngine::loadFont(std::string filename, unsigned int size) {
			return *faces.emplace_back(std::make_unique<Font>(ft, filename, size));
		}
	}
}