#include <mainframe/render/stencil.h>
#include <mainframe/render/font.h>
#include <mainframe/numbers/pi.h>

#include <GL/glew.h>
#include <freetype-gl/freetype-gl.h>
#include <stdexcept>

namespace mainframe {
	namespace render {
		Texture Stencil::texPixel({1, 1}, Colors::White);

		Stencil::VerticeData::VerticeData(float _x, float _y, float _z, float _u, float _v, float _r, float _g, float _b, float _a) : x(_x), y(_y), z(_z), u(_u), v(_v), r(_r), g(_g), b(_b), a(_a) {

		}

		void Stencil::setShader(int shader) {
			if (currentShaderHandle != shader) draw();
			currentShaderHandle = shader;
		}

		void Stencil::setShader(const Shader& shader) {
			setShader(shader.getHandle());
		}

		int Stencil::getShader() {
			return currentShaderHandle;
		}

		void Stencil::setTexture(unsigned int tex) {
			if (currentTextureHandle != tex) draw();
			currentTextureHandle = tex;
		}

		void Stencil::setTexture(const Texture& tex) {
			setTexture(tex.getHandle());
		}

		unsigned int Stencil::getTexture() {
			return currentTextureHandle;
		}

		void Stencil::pushVertice(const mainframe::math::Vector2& pos, const mainframe::math::Vector2& uv, const Color& col) {
			vertices.emplace_back(
				// pos
				(pos.x + offset.x) / windowSize.x * 2 - 1,
				((pos.y + offset.y) / windowSize.y * 2 - 1) * -1,
				0,

				// uv
				uv.x,
				uv.y,

				// color
				col.r,
				col.g,
				col.b,
				col.a
			);
		}

		void Stencil::pushIndices(unsigned int a, unsigned int b, unsigned int c) {
			auto pos = vertices.size();

			indices.push_back(pos - a);
			indices.push_back(pos - b);
			indices.push_back(pos - c);
		}

		void Stencil::drawTriangle(const mainframe::math::Vector2& a, const mainframe::math::Vector2& aUV, const Color& colA, const mainframe::math::Vector2& b, const mainframe::math::Vector2& bUV, const Color& colB, const mainframe::math::Vector2& c, const mainframe::math::Vector2& cUV, const Color& colC) {
			if (colA.a == 0 && colB.a == 0 && colC.a == 0) return;

			setTexture(texPixel);
			setShader(shader2D);

			pushVertice(a + offset, aUV, colA);
			pushVertice(b + offset, bUV, colB);
			pushVertice(c + offset, cUV, colC);
			pushIndices(3, 2, 1);
		}

		void Stencil::drawPolygon(const Polygon& poly) {
			setTexture(texPixel);
			setShader(shader2D);

			auto spos = vertices.size();
			for (auto v : poly.verts)
				pushVertice(v.pos + offset, v.uv, v.col);

			for (auto i : poly.indices)
				indices.push_back(spos + i);
		}

		void Stencil::drawCircleOutlined(const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size, size_t roundness, float borderSize, Color col) {
			if (col.a == 0) return;

			setTexture(texPixel);
			setShader(shader2D);

			auto radius = size / 2;
			auto radiusBorder = radius - borderSize;
			auto targetPos = pos + radius;

			float space = numbers::pi<float> / roundness * 2;


			for (size_t i = 0; i < roundness;) {
				math::Vector2 a = targetPos + math::Vector2::cosSin(space * i) * radiusBorder;
				math::Vector2 b = targetPos + math::Vector2::cosSin(space * i) * radius;

				i++;
				math::Vector2 c = targetPos + math::Vector2::cosSin(space * i) * radiusBorder;
				math::Vector2 d = targetPos + math::Vector2::cosSin(space * i) * radius;

				drawTriangle(
					a, {}, col,
					b, {}, col,
					c, {}, col
				);

				drawTriangle(
					b, {}, col,
					d, {}, col,
					c, {}, col
				);
			}
		}

		void Stencil::drawCircle(const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size, size_t roundness, Color col) {
			if (col.a == 0) return;

			setTexture(texPixel);
			setShader(shader2D);

			auto radius = size / 2;
			auto targetPos = pos + radius;

			float space = numbers::pi<float> / roundness * 2;

			for (size_t i = 0; i < roundness;) {
				math::Vector2 b = targetPos + math::Vector2::cosSin(space * i) * radius;
				math::Vector2 c = targetPos + math::Vector2::cosSin(space * ++i) * radius;

				drawTriangle(
					targetPos, {}, col,
					b, {}, col,
					c, {}, col);
			}
		}

		void Stencil::drawBoxOutlined(const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size, float borderSize, Color col) {
			if (col.a == 0) return;

			setTexture(texPixel);
			setShader(shader2D);

			// top 4
			pushVertice(pos, {0, 0}, col);
			pushVertice(pos + math::Vector2(size.x, 0), {0, 0}, col);
			pushVertice(pos + borderSize, {0, 0}, col);
			pushVertice(pos + math::Vector2(size.x - borderSize, borderSize), {0, 0}, col);

			// bottom 4
			pushVertice(pos + math::Vector2(borderSize, size.y - borderSize), {0, 0}, col);
			pushVertice(pos + math::Vector2(0, size.y), {0, 0}, col);
			pushVertice(pos + size - borderSize, {0, 0}, col);
			pushVertice(pos + size, {0, 0}, col);

			// top
			pushIndices(8, 7, 6);
			pushIndices(7, 6, 5);

			// left
			pushIndices(8, 6, 4);
			pushIndices(4, 3, 8);

			// right
			pushIndices(7, 5, 2);
			pushIndices(2, 1, 7);

			// bottom
			pushIndices(4, 3, 2);
			pushIndices(3, 2, 1);
		}

		void Stencil::drawTexture(mainframe::math::Vector2 pos, mainframe::math::Vector2 size, const Texture& tex, Color col, mainframe::math::Vector2 uvStart, mainframe::math::Vector2 uvEnd, float rotation, const mainframe::math::Vector2& origin) {
			if (col.a == 0) return;

			pos += offset;

			// TODO: idunno how to support clipping with rotation UV magic
			// move to shader? should also speed up this process here as the GPU already computes the coords for us
			// needs fallback for EGL then tho....

			if (rotation == 0) {
				auto& clip = getClippingRect();
				auto pixelSize = (uvEnd - uvStart) / size;

				if (pos.x > clip.x + clip.w) return;
				if (pos.y > clip.y + clip.h) return;
				if (pos.x + size.x < clip.x) return;
				if (pos.y + size.y < clip.y) return;

				if (pos.x < clip.x) {
					float diff = clip.x - pos.x;

					uvStart.x += diff * pixelSize.x;
					size.x -= diff;
					pos.x = clip.x;
				}

				if (pos.y < clip.y) {
					float diff = clip.y - pos.y;

					uvStart.y += diff * pixelSize.y;
					size.y -= diff;
					pos.y = clip.y;
				}

				float maxX = (clip.x + clip.w);
				float maxY = (clip.y + clip.h);

				if (pos.x + size.x > maxX) {
					float diff = pos.x + size.x - clip.x - clip.w;
					size.x = maxX - pos.x;

					uvEnd.x -= diff * pixelSize.x;
				}

				if (pos.y + size.y > maxY) {
					float diff = pos.y + size.y - clip.y - clip.h;
					size.y = maxY - pos.y;

					uvEnd.y -= diff * pixelSize.y;
				}
			}

			if (size == 0) return;

			math::Vector2 b = pos;
			math::Vector2 c = pos;

			b.x += size.x;
			c.y += size.y;

			setTexture(tex);
			setShader(shader2D);

			auto rotOrigin = origin.isNaN() ? pos + size / 2 : origin + offset;

			auto vertA = pos.rotateAroundOrigin(rotation, rotOrigin);
			auto vertB = b.rotateAroundOrigin(rotation, rotOrigin);
			auto vertC = c.rotateAroundOrigin(rotation, rotOrigin);
			auto vertD = (pos + size).rotateAroundOrigin(rotation, rotOrigin);


			pushVertice(vertA, uvStart, col);
			pushVertice(b.rotateAroundOrigin(rotation, rotOrigin), {uvEnd.x, uvStart.y}, col);
			pushVertice(c.rotateAroundOrigin(rotation, rotOrigin), {uvStart.x, uvEnd.y}, col);
			pushVertice((pos + size).rotateAroundOrigin(rotation, rotOrigin), uvEnd, col);

			pushIndices(4, 3, 2);
			pushIndices(3, 2, 1);
		}

		void Stencil::drawBox(const math::Vector2& pos, const math::Vector2& size, Color col) {
			drawTexture(pos, size, texPixel, col);
		}

		void Stencil::drawText(const Font& font, const std::string& text, const math::Vector2& pos, Color col, TextAlignment alignx, TextAlignment aligny, float rotation, const mainframe::math::Vector2& origin) {
			if (col.a == 0 || text.empty()) return;

			setTexture(font.tex);
			setShader(shader2DText);

			math::Vector2 startpos = pos;

			math::Vector2 tsize = font.getStringSize(text);
			if (alignx != TextAlignment::Left || aligny != TextAlignment::Left) {
				switch (alignx) {
					case TextAlignment::Left: break;
					case TextAlignment::Center: startpos.x -= tsize.x / 2; break;
					case TextAlignment::Right: startpos.x -= tsize.x; break;
				}

				switch (aligny) {
					case TextAlignment::Left: break;
					case TextAlignment::Center: startpos.y -= tsize.y / 2; break;
					case TextAlignment::Right: startpos.y -= tsize.y; break;
				}
			}

			float lineheight = font.getLineHeight();
			startpos.y -= lineheight / 4;

			math::Vector2 curpos = startpos;

			auto rotOrigin = origin.isNaN() ? pos + tsize / 2 + offset : origin;

			const ftgl::texture_glyph_t* prevGlyph = nullptr;
			for (auto l : text) {
				if (l == '\n') {
					curpos.y += lineheight;
					curpos.x = startpos.x;

					prevGlyph = nullptr;
					continue;
				}

				auto glyph = font.getGlyph(l);
				if (glyph == nullptr) continue;

				if (prevGlyph != nullptr) {
					curpos.x += font.getKerning(glyph, prevGlyph);
				}

				drawTexture(
					{curpos.x + glyph->offset_x, curpos.y + lineheight - glyph->offset_y},
					{static_cast<float>(glyph->width), static_cast<float>(glyph->height)},
					font.tex,
					col,
					{glyph->s0, glyph->t0},
					{glyph->s1, glyph->t1},
					rotation,
					origin
				);

				curpos.x += glyph->advance_x;
				prevGlyph = glyph;
			}
		}

		bool Stencil::getDisableDept() {
			return deptDisabled;
		}

		void Stencil::setDisableDept(bool disabled) {
			deptDisabled = disabled;
		}

		Stencil::Stencil() {
#ifndef MAINFRAME_EGL
			auto initShader = [&](auto& s) {
				s.link();
				s.use();

				auto handle = s.getHandle();

				glBindFragDataLocation(handle, 0, "outColor");
				glUniform1i(glGetUniformLocation(handle, "tex"), 0);

				GLint posAttrib = glGetAttribLocation(handle, "position");
				glEnableVertexAttribArray(posAttrib);
				glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);

				GLint tposAttrib = glGetAttribLocation(handle, "texpos");
				glEnableVertexAttribArray(tposAttrib);
				glVertexAttribPointer(tposAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 3));

				GLint colAttrib = glGetAttribLocation(handle, "color");
				glEnableVertexAttribArray(colAttrib);
				glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 5));
			};

			shader2DText.attachRaw("#version 150\nout vec4 outColor;\n\nin vec4 output_color;\nin vec2 output_texpos;\n\nuniform sampler2D tex;\n\nvoid main(){outColor = texture(tex, output_texpos) * output_color;\nif (outColor.a == 0.0) discard;\n}\n", GL_FRAGMENT_SHADER);
			shader2DText.attachRaw("#version 150\nin vec3 position;\nin vec2 texpos;\nin vec4 color;\n\nout vec2 output_texpos;\nout vec4 output_color;\nvoid main() {\ngl_Position = vec4(position, 1.0);\noutput_color = color;\noutput_texpos = texpos;\n}\n", GL_VERTEX_SHADER);
			initShader(shader2DText);


			shader2D.attachRaw("#version 150\nout vec4 outColor;\n\nin vec4 output_color;\nin vec2 output_texpos;\n\nuniform sampler2D tex;\n\nvoid main(){\noutColor = texture(tex, output_texpos) * output_color;\n\nif (outColor.a == 0.0) discard;\n}\n", GL_FRAGMENT_SHADER);
			shader2D.attachRaw("#version 150\nin vec3 position;\nin vec2 texpos;\nin vec4 color;\n\nout vec2 output_texpos;\nout vec4 output_color;\nvoid main() {\ngl_Position = vec4(position, 1.0);\noutput_color = color;\noutput_texpos = texpos;\n}\n", GL_VERTEX_SHADER);
			initShader(shader2D);
#endif

			if (texPixel.getHandle() == -1) {
				texPixel.upload();
			}

			// filled in later by windowSize
			clippingRects.emplace_back(0.0f, 0.0f, 0.0f, 0.0f);
		}

		void Stencil::pushClipping(const mainframe::math::AABB& rect) {
			mainframe::math::AABB newrect = {rect.x + offset.x, rect.y + offset.y, rect.w, rect.h};
			newrect = newrect.clamp(clippingRects.back());

			clippingRects.push_back(newrect);
		}

		void Stencil::popClipping() {
			if (clippingRects.size() == 1) throw std::runtime_error("no more clippings to pop");

			clippingRects.pop_back();
		}

		bool Stencil::getClippingEnabled() const {
			return !clippingRects.empty();
		}

		const mainframe::math::AABB& Stencil::getClippingRect() const {
			return clippingRects.back();
		}

		void Stencil::pushOffset(const mainframe::math::Vector2& _offset) {
			offsets.push_back(_offset);
			offset += _offset;
		}

		void Stencil::popOffset() {
			offset -= offsets.back();
			offsets.pop_back();
		}

		const mainframe::math::Vector2& Stencil::getOffset() const {
			return offset;
		}

		void Stencil::setWindowSize(const mainframe::math::Vector2i& size) {
			windowSize = size;

			auto& r = clippingRects.front();
			r.w = windowSize.x;
			r.h = windowSize.y;
		}

		mainframe::math::Vector2i Stencil::getWindowSize() const {
			return windowSize;
		}

		void Stencil::draw() {
			if (vertices.empty()) return;

			if (getDisableDept()) glDisable(GL_DEPTH_TEST);

			/*
			if (this->CaptureBuffer != nullptr) {
				RenderBufferChunk c;
				c.ShaderHandle = this->CurrentShader;
				c.TextureHandle = this->CurrentTexture;

				c.VerticeDataCount = this->VerticeDataCount;
				c.VerticeData = new _vertexData[c.VerticeDataCount];
				memcpy(c.VerticeData, this->VerticeData, c.VerticeDataCount * sizeof(_vertexData));

				c.IndiceDataCount = this->IndiceDataCount;
				c.IndiceData = new GLuint[c.IndiceDataCount];
				memcpy(c.IndiceData, this->IndiceData, c.IndiceDataCount * sizeof(GLuint));

				this->CaptureBuffer->Chunks.Add(c);
			}
			*/

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, currentTextureHandle);

#ifndef MAINFRAME_EGL
			glUseProgram(currentShaderHandle);


			glBindVertexArray(buffer.getVao());

			glBindBuffer(GL_ARRAY_BUFFER, buffer.getVbo());
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * 9 * sizeof(float), vertices.data(), GL_STREAM_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.getEbo());
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STREAM_DRAW);

			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
#else
			glLoadIdentity();

			std::vector<float> rawtexcoords;
			std::vector<float> rawvertices;
			std::vector<float> rawcolors;

			rawtexcoords.reserve(2 * indices.size());
			rawvertices.reserve(2 * indices.size());
			rawcolors.reserve(4 * indices.size());

			for (size_t i = 0; i < indices.size(); i++) {
				auto& v = vertices[indices[i]];

				rawcolors.push_back(v.r);
				rawcolors.push_back(v.g);
				rawcolors.push_back(v.b);
				rawcolors.push_back(v.a);

				rawtexcoords.push_back(v.u);
				rawtexcoords.push_back(v.v);

				rawvertices.push_back(v.x);
				rawvertices.push_back(v.y);
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_COLOR_MATERIAL);

			glVertexPointer(2, GL_FLOAT, 0, rawvertices.data());
			glTexCoordPointer(2, GL_FLOAT, 0, rawtexcoords.data());
			glColorPointer(4, GL_FLOAT, 0, rawcolors.data());

			glDrawArrays(GL_TRIANGLES, 0, indices.size() / 3);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);

			glDisable(GL_TEXTURE_2D);
			glDisable(GL_COLOR_MATERIAL);
#endif

			if (getDisableDept()) glEnable(GL_DEPTH_TEST);

			vertices.clear();
			indices.clear();
		}
	}
}