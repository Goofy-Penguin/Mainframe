#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <mainframe/math/vector2.h>

namespace mainframe {
	namespace render {
		struct Glyph {
			FT_ULong codepoint;
			FT_UInt glyphIndex;
			mainframe::math::Vector2 bearing;
			mainframe::math::Vector2 advance;
			mainframe::math::Vector2 textureTopLeft;
			mainframe::math::Vector2 textureBottomRight;
			mainframe::math::Vector2i size;
		};
	}
}
