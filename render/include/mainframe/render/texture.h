#pragma once

#include <mainframe/math/vector2.h>
#include <mainframe/render/color.h>
#include <vector>
#include <string>

namespace mainframe {
	namespace render {
		class Texture {
		private:
			mainframe::math::Vector2i size;
			std::vector<Color> pixels;
			unsigned int handle = -1;

			void checkPixels() const;

		public:
			Texture() = default;
			Texture(const Texture& t);
			Texture(const mainframe::math::Vector2i& initsize, const mainframe::render::Color& bgcol = Colors::Transparent);
			Texture(const std::string& fileName);
			~Texture();

			const mainframe::math::Vector2i& getSize() const;
			void resize(const mainframe::math::Vector2i& newsize);

			mainframe::render::Color& getPixel(unsigned x, unsigned y);
			const mainframe::render::Color& getPixel(unsigned x, unsigned y) const;
			mainframe::render::Color& getPixel(const mainframe::math::Vector2i& pos);
			const mainframe::render::Color& getPixel(const mainframe::math::Vector2i& pos) const;

			void setPixel(const mainframe::math::Vector2i& pos, const mainframe::render::Color& col);

			void bind();
			void upload();
			void use();
			void use() const;
			void clear(const mainframe::render::Color& bgcol = Colors::Transparent);
			void unloadPixels();

			std::vector<Color>& data();
			const std::vector<Color>& data() const;
			unsigned int getHandle() const;
		};
	}
}