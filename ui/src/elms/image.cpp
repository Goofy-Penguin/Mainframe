#include <mainframe/ui/elms/image.h>

namespace mainframe {
	namespace ui {
		void Image::setImage(const render::Texture& texture) {
			tex = texture;
			if (tex.getHandle() == -1) tex.upload();
		}

		void Image::setImage(const std::string& filename) {
			tex = filename;
			tex.upload();
			tex.unloadPixels();
		}

		const render::Color& Image::getColor() {
			return color;
		}

		void Image::setColor(const render::Color& color_) {
			color = color_;
		}

		void Image::draw(render::Stencil& stencil) {
			stencil.drawTexture({0, 0}, getSize(), tex, color);
		}
	}
}