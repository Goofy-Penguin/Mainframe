#include <mainframe/render/texture.h>
#include <mainframe/libs/lodepng.h>

#include <stdexcept>
#include <GL/glew.h>

namespace mainframe {
	namespace render {
		Texture::Texture(const Texture& t) {

		}

		Texture::Texture(const mainframe::math::Vector2i& initsize, const mainframe::render::Color& bgcol) {
			pixels.resize(initsize.y * initsize.x);
			size = initsize;
			for (auto& elm : pixels) {
				elm = bgcol;
			}
		}

		Texture::Texture(const std::string& fileName) {
			unsigned w = static_cast<unsigned>(size.x);
			unsigned h = static_cast<unsigned>(size.y);

			std::vector<unsigned char> image;
			unsigned error = lodepng::decode(image, w, h, fileName);
			if (error != 0) throw std::runtime_error("error loading png");

			pixels.resize(h * w);
			size.x = static_cast<float>(w);
			size.y = static_cast<float>(h);

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

		Texture::~Texture() {
			if (handle == -1) return;
			glDeleteTextures(1, &handle);
		}

		const mainframe::math::Vector2i& Texture::getSize() const {
			return size;
		}

		unsigned int Texture::getHandle() const {
			return handle;
		}

		void Texture::resize(const mainframe::math::Vector2i& newsize) {
			std::vector<Color> newpixels;
			newpixels.resize(newsize.y * newsize.x);

			if (size.x > 0 && size.y > 0) {
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

		void Texture::bind() {
			if (handle != -1) return;

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

			glGenTextures(1, &handle);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, handle);

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

			if (handle == -1) bind();

			glBindTexture(GL_TEXTURE_2D, handle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_FLOAT, pixels.data());
		}

		void Texture::use() {
			if (handle == -1) {
				upload();
				return;
			}

			glBindTexture(GL_TEXTURE_2D, handle);
		}

		void Texture::use() const {
			glBindTexture(GL_TEXTURE_2D, handle);
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