#include <mainframe/render/textengine.h>

#include <stdexcept>
#include <fmt/format.h>

namespace mainframe::render {
	TextEngine::TextEngine() {
		init();
	}

	void TextEngine::init() {
		if (ft != nullptr) return;
		if (FT_Init_FreeType(&ft) != 0) throw std::runtime_error("Error: failed to initialize freetype");

		// FT_LCD_FILTER_LIGHT   is (0x00, 0x55, 0x56, 0x55, 0x00)
		// FT_LCD_FILTER_DEFAULT is (0x10, 0x40, 0x70, 0x40, 0x10)
		unsigned char lcd_weights[5];
		lcd_weights[0] = 0x00;
		lcd_weights[1] = 0x55;
		lcd_weights[2] = 0x56;
		lcd_weights[3] = 0x55;
		lcd_weights[4] = 0x00;

		FT_Library_SetLcdFilter(ft, FT_LCD_FILTER_LIGHT);
		FT_Library_SetLcdFilterWeights(ft, lcd_weights);
	}

	void TextEngine::unload() {
		if (ft == nullptr) return;

		// Clear faces now so they can clean up before the freetype library
		// is cleaned up.
		faces.clear();

		if (FT_Done_FreeType(ft) != 0) throw std::runtime_error("Error: failed to clean up freetype");
		ft = nullptr;
	}

	TextEngine::~TextEngine() {
		unload();
	}

	Font& TextEngine::loadFont(std::string filename, unsigned int size, uint32_t flags) {
		return *faces.emplace_back(std::make_unique<Font>(ft, filename, size, flags));
	}
}
