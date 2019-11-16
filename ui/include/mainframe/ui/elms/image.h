#pragma once

#include <mainframe/ui/element.h>
#include <mainframe/render/texture.h>

namespace mainframe {
	namespace ui {
		class Image : public Element {
			render::Texture tex;
			render::Color color = render::Colors::White;

		public:
			void setImage(const std::string& filename);
			void setImage(const render::Texture& texture);

			void setColor(const render::Color& color_);
			const render::Color& getColor();

			virtual void draw(render::Stencil& stencil) override;
		};
	}
}