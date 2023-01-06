#pragma once

#include <mainframe/math/vector2.h>
#include <mainframe/render/color.h>

#include <memory>
#include <vector>
#include <string>
#include <climits>

namespace mainframe::render {
	class TextureHandle {
	public:
		unsigned int glHandle = UINT_MAX;

		void reset();

		TextureHandle() = default;
		~TextureHandle();
	};

	class Texture {
	private:
		unsigned int quality = 0x2600; // GL_NEAREST
		unsigned int wrap = 0x2901; // GL_REPEAT

		mainframe::math::Vector2i originalSize;
		mainframe::math::Vector2i size;
		std::vector<Color> pixels;
		std::shared_ptr<TextureHandle> handle = std::make_shared<TextureHandle>();

		void checkPixels() const;

	public:
		unsigned int blend_src = 0x0303; // GL_SRC_ALPHA
		unsigned int blend_dest = 0x0302; // GL_ONE_MINUS_SRC_ALPHA

		Texture() = default;
		Texture(const Texture& t) = default;
		Texture(const mainframe::math::Vector2i& initsize, const mainframe::render::Color& bgcol = Colors::Transparent);
		Texture(const std::string& fileName);

		const mainframe::math::Vector2i& getSize() const;
		const mainframe::math::Vector2i& getOriginalSize() const;
		void resize(const mainframe::math::Vector2i& newsize);

		mainframe::render::Color& getPixel(unsigned x, unsigned y);
		const mainframe::render::Color& getPixel(unsigned x, unsigned y) const;
		mainframe::render::Color& getPixel(const mainframe::math::Vector2i& pos);
		const mainframe::render::Color& getPixel(const mainframe::math::Vector2i& pos) const;

		void setPixel(const mainframe::math::Vector2i& pos, const mainframe::render::Color& col);
		void setPixels(const mainframe::math::Vector2i& size_, const std::vector<Color>& data);
		void setMaxAnisotropy();

		void bind(int index = 0);
		void upload();
		void setQuality(unsigned int quality);
		void setWrap(unsigned int wrap);
		void setBlend(unsigned int blend_src, unsigned int blend_dest);

		void use();
		void use() const;
		void clear(const mainframe::render::Color& bgcol = Colors::Transparent);
		void unloadPixels();

		std::vector<uint8_t> toRGBABytes();
		void fromRGBABytes(std::vector<uint8_t> buff);

		std::vector<Color>& data();
		const std::vector<Color>& data() const;
		unsigned int getHandle() const;
		std::shared_ptr<TextureHandle>& getSharedHandle();
		void setSharedHandle(std::shared_ptr<TextureHandle>& handle_);
	};
}
