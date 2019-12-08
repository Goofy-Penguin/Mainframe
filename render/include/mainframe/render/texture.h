#pragma once

#include <mainframe/math/vector2.h>
#include <mainframe/render/color.h>
#include <memory>
#include <vector>
#include <string>
#include <climits>

namespace mainframe {
	namespace render {
		class TextureHandle {
		public:
			unsigned int glHandle = UINT_MAX;

			void reset();

			TextureHandle() = default;
			~TextureHandle();
		};

		class Texture {
		private:
			mainframe::math::Vector2i size;
			std::vector<Color> pixels;
			std::shared_ptr<TextureHandle> handle = std::make_shared<TextureHandle>();

			void checkPixels() const;

		public:
			Texture() = default;
			Texture(const Texture& t) = default;
			Texture(const mainframe::math::Vector2i& initsize, const mainframe::render::Color& bgcol = Colors::Transparent);
			Texture(const std::string& fileName);

			const mainframe::math::Vector2i& getSize() const;
			void resize(const mainframe::math::Vector2i& newsize);

			mainframe::render::Color& getPixel(unsigned x, unsigned y);
			const mainframe::render::Color& getPixel(unsigned x, unsigned y) const;
			mainframe::render::Color& getPixel(const mainframe::math::Vector2i& pos);
			const mainframe::render::Color& getPixel(const mainframe::math::Vector2i& pos) const;

			void setPixel(const mainframe::math::Vector2i& pos, const mainframe::render::Color& col);
			void setPixels(const mainframe::math::Vector2i& size_, const std::vector<Color>& data);

			void bind();
			void upload();
			void use();
			void use() const;
			void clear(const mainframe::render::Color& bgcol = Colors::Transparent);
			void unloadPixels();

			std::vector<Color>& data();
			const std::vector<Color>& data() const;
			unsigned int getHandle() const;
			std::shared_ptr<TextureHandle>& getSharedHandle();
			void setSharedHandle(std::shared_ptr<TextureHandle>& handle_);
		};
	}
}