#include <mainframe/render/texture.h>
#include <lodepng.h>

#include <stdexcept>
#include <GL/glew.h>

namespace mainframe {
	namespace render {
		void TextureHandle::reset() {
			if (glHandle == -1) return;
			glDeleteTextures(1, &glHandle);
			glHandle = -1;
		}

		TextureHandle::~TextureHandle() {
			reset();
		}

		Texture::Texture(const mainframe::math::Vector2i& initsize, const mainframe::render::Color& bgcol) {
			pixels.resize(initsize.y * initsize.x);
			size = initsize;
			for (auto& elm : pixels) {
				elm = bgcol;
			}
		}

		void Texture::fromRGBABytes(std::vector<uint8_t> buff) {
			if (buff.size() / 4 != pixels.size()) {
				throw std::runtime_error("RGBA buffer does not match image size");
			}

			for (size_t i = 0; i < buff.size(); i += 4) {
				auto& col = pixels[i / 4];

				col.r = static_cast<float>(buff[i + 0]) / 255.0f;
				col.g = static_cast<float>(buff[i + 1]) / 255.0f;
				col.b = static_cast<float>(buff[i + 2]) / 255.0f;
				col.a = static_cast<float>(buff[i + 3]) / 255.0f;
			}
		}

		std::vector<uint8_t> Texture::toRGBABytes() {
			std::vector<uint8_t> ret;
			ret.resize(size.x * size.y * 4);

			for (size_t i = 0; i < ret.size(); i += 4) {
				auto& col = pixels[i / 4];

				ret[i + 0] = static_cast<uint8_t>(col.r * 255);
				ret[i + 1] = static_cast<uint8_t>(col.g * 255);
				ret[i + 2] = static_cast<uint8_t>(col.b * 255);
				ret[i + 3] = static_cast<uint8_t>(col.a * 255);
			}

			return ret;
		}

		Texture::Texture(const std::string& fileName) {
			unsigned w = static_cast<unsigned>(size.x);
			unsigned h = static_cast<unsigned>(size.y);

			std::vector<unsigned char> image;
			unsigned error = lodepng::decode(image, w, h, fileName);
			if (error != 0) throw std::runtime_error("error loading png");

			pixels.resize(h * w);
			size.x = static_cast<int>(w);
			size.y = static_cast<int>(h);

			for (unsigned y = 0; y < h; y++) {
				for (unsigned x = 0; x < w; x++) {
					int offset = y * w * 4 + x * 4;

					auto& p = getPixel(x, y);
					p.r = static_cast<float>(image[offset++]) / 255;
					p.g = static_cast<float>(image[offset++]) / 255;
					p.b = static_cast<float>(image[offset++]) / 255;
					p.a = static_cast<float>(image[offset++]) / 255;
				}
			}
		}

		const mainframe::math::Vector2i& Texture::getSize() const {
			return size;
		}

		unsigned int Texture::getHandle() const {
			return handle->glHandle;
		}

		void Texture::setSharedHandle(std::shared_ptr<TextureHandle>& handle_) {
			handle = handle_;
		}

		std::shared_ptr<TextureHandle>& Texture::getSharedHandle() {
			return handle;
		}

		void Texture::resize(const mainframe::math::Vector2i& newsize) {
			std::vector<Color> newpixels;
			newpixels.resize(newsize.y * newsize.x);

			if (!pixels.empty() && size.x > 0 && size.y > 0) {
				auto wperc = static_cast<float>(newsize.x) / static_cast<float>(size.x);
				auto hperc = static_cast<float>(newsize.y) / static_cast<float>(size.y);

				for (int cy = 0; cy < newsize.y; cy++) {
					for (int cx = 0; cx < newsize.x; cx++) {
						int pixel = cy * newsize.x + cx;
						int nearestMatch = static_cast<int>(cy / hperc)* size.x + static_cast<int>(cx / wperc);

						newpixels[pixel] = pixels[nearestMatch];
					}
				}
			}

			size = newsize;
			pixels = newpixels;
		}

		void Texture::checkPixels() const {
			if (pixels.empty() && size != 0) {
				throw std::runtime_error("trying to access pixels, but got empty");
			}
		}

		mainframe::render::Color& Texture::getPixel(unsigned x, unsigned y) {
			checkPixels();
			return pixels[y * size.x + x];
		}

		const mainframe::render::Color& Texture::getPixel(unsigned x, unsigned y) const {
			checkPixels();
			return pixels[y * size.x + x];
		}

		const mainframe::render::Color& Texture::getPixel(const mainframe::math::Vector2i& pos) const {
			return getPixel(pos.x, pos.y);
		}

		mainframe::render::Color& Texture::getPixel(const mainframe::math::Vector2i& pos) {
			return getPixel(pos.x, pos.y);
		}

		void Texture::setPixel(const mainframe::math::Vector2i& pos, const mainframe::render::Color& col) {
			getPixel(pos) = col;
		}

		void Texture::setPixels(const mainframe::math::Vector2i& size_, const std::vector<Color>& data) {
			pixels = data;
			size = size_;
		}

		void Texture::bind() {
			auto& glhandle = handle->glHandle;
			if (glhandle != UINT_MAX) return;

			// textures in GL must be power of 2
			mainframe::math::Vector2i pow = size;
			if (pow.x != 0 && (pow.x & (pow.x - 1)) != 0) {
				pow.x--;
				pow.x |= pow.x >> 1;
				pow.x |= pow.x >> 2;
				pow.x |= pow.x >> 4;
				pow.x |= pow.x >> 8;
				pow.x |= pow.x >> 16;
				pow.x++;
			}

			if (pow.y != 0 && (pow.y & (pow.y - 1)) != 0) {
				pow.y--;
				pow.y |= pow.y >> 1;
				pow.y |= pow.y >> 2;
				pow.y |= pow.y >> 4;
				pow.y |= pow.y >> 8;
				pow.y |= pow.y >> 16;
				pow.y++;
			}


			if (pow != size) {
				resize(pow);
			}

			glGenTextures(1, &glhandle);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, glhandle);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		std::vector<Color>& Texture::data() {
			return pixels;
		}

		const std::vector<Color>& Texture::data() const {
			return pixels;
		}

		void Texture::upload() {
			checkPixels();

			auto& glhandle = handle->glHandle;
			if (glhandle == UINT_MAX) bind();

			glBindTexture(GL_TEXTURE_2D, glhandle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_FLOAT, pixels.data());
		}

		void Texture::use() {
			auto& glhandle = handle->glHandle;
			if (glhandle == UINT_MAX) {
				upload();
				return;
			}

			glBindTexture(GL_TEXTURE_2D, glhandle);
		}

		void Texture::use() const {
			glBindTexture(GL_TEXTURE_2D, getHandle());
		}

		void Texture::clear(const mainframe::render::Color& bgcol) {
			for (auto& elm : pixels) {
				elm = bgcol;
			}
		}

		void Texture::unloadPixels() {
			pixels.clear();
		}
	}
}
