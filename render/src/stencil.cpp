#include <mainframe/render/stencil.h>
#include <mainframe/render/font.h>

#include <GL/glew.h>
#include <stdexcept>
#include <utf8.h>

namespace mainframe::render {
	int& Stencil::getPixelTextureGetRef() {
		thread_local int ref = 0;
		return ref;
	}

	void Stencil::getPixelTextureIncRef() {
		getPixelTextureGetRef()++;
	}

	void Stencil::getPixelTextureDecRef() {
		auto& ref = getPixelTextureGetRef();
		ref--;

		if (ref > 0) return;
		getPixelTexture().getSharedHandle()->reset();
	}

	Texture& Stencil::getPixelTexture() {
		thread_local Texture pixel = {{1, 1}, Colors::White};

		return pixel;
	}

	Stencil::VerticeData::VerticeData(float _x, float _y, float _z, float _u, float _v, float _r, float _g, float _b, float _a) : x(_x), y(_y), z(_z), u(_u), v(_v), r(_r), g(_g), b(_b), a(_a) {}

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
			pos.x / windowSize.x * 2 - 1,
			(pos.y  / windowSize.y * 2 - 1) * -1,
			0.0f,

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


	void Stencil::pushLocalOffset() {
		oldOffset = offset;
		offset = {};
	}

	void Stencil::popLocalOffset() {
		offset = oldOffset;
	}

	void Stencil::pushDisableClip() {
		clipDisabled = true;
	}

	void Stencil::popDisableClip() {
		clipDisabled = false;
	}

	void Stencil::pushIndices(unsigned int a, unsigned int b, unsigned int c) {
		auto pos = static_cast<unsigned int>(vertices.size());

		indices.push_back(pos - a);
		indices.push_back(pos - b);
		indices.push_back(pos - c);
	}

	void Stencil::drawTriangle(const mainframe::math::Vector2& a, const mainframe::math::Vector2& aUV, const Color& colA, const mainframe::math::Vector2& b, const mainframe::math::Vector2& bUV, const Color& colB, const mainframe::math::Vector2& c, const mainframe::math::Vector2& cUV, const Color& colC) {
		if (colA.a == 0 && colB.a == 0 && colC.a == 0) return;

		setTexture(getPixelTexture());
		if(shaderOverride) setShader(shader2D);

		pushVertice(a + offset, aUV, colA);
		pushVertice(b + offset, bUV, colB);
		pushVertice(c + offset, cUV, colC);
		pushIndices(3, 2, 1);
	}

	void Stencil::drawPolygon(const Polygon& poly) {
		setTexture(getPixelTexture());
		if(shaderOverride) setShader(shader2D);

		auto spos = static_cast<unsigned int>(vertices.size());
		for (auto v : poly.verts)
			pushVertice(v.pos + offset, v.uv, v.col);

		for (auto i : poly.indices)
			indices.push_back(spos + i);
	}

	void Stencil::drawCircleOutline(const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size, size_t roundness, float borderSize, Color col, float angleStart, float maxAngle) {
		if (col.a == 0) return;

		setTexture(getPixelTexture());
		if(shaderOverride) setShader(shader2D);

		auto radius = size / 2;
		auto radiusBorder = radius - borderSize;
		auto targetPos = pos + radius;

		float space = numbers::pi<float> / roundness * 2;

		for (size_t i = 0; i < roundness;) {
			auto angStart = space * i++ + angleStart;
			auto angEnd = angStart + space;
			if (angEnd > maxAngle) break;

			math::Vector2 a = targetPos + math::Vector2::cosSin(angStart) * radiusBorder;
			math::Vector2 b = targetPos + math::Vector2::cosSin(angStart) * radius;

			math::Vector2 c = targetPos + math::Vector2::cosSin(angEnd) * radiusBorder;
			math::Vector2 d = targetPos + math::Vector2::cosSin(angEnd) * radius;

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

	void Stencil::drawCircle(const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size, size_t roundness, Color col, float angleStart, float maxAngle) {
		if (col.a == 0) return;

		setTexture(getPixelTexture());
		if(shaderOverride) setShader(shader2D);

		auto radius = size / 2;
		auto targetPos = pos + radius;

		float space = numbers::pi<float> / roundness * 2;

		for (size_t i = 0; i < roundness;) {
			auto ang = space * i++ + angleStart;
			if (ang + space > maxAngle) break;

			math::Vector2 b = targetPos + math::Vector2::cosSin(ang) * radius;
			math::Vector2 c = targetPos + math::Vector2::cosSin(ang + space) * radius;

			drawTriangle(
				targetPos, {}, col,
				b, {}, col,
				c, {}, col);
		}
	}

// auto rotated = newpos.rotateAroundOrigin(rotation, orginPos);
	void Stencil::drawBoxOutlined(mainframe::math::Vector2 pos, const mainframe::math::Vector2& size, const mainframe::math::Vector2& borderSize, Color col, float rotation, const mainframe::math::Vector2& rotationOrigin) {
		if (col.a == 0) return;

		pos += offset;

		setTexture(getPixelTexture());
		if(shaderOverride) setShader(shader2D);

		// top 4
		math::Vector2 A = pos;
		math::Vector2 B = A + math::Vector2(size.x, 0);
		math::Vector2 C = A + borderSize;
		math::Vector2 D = A + math::Vector2(size.x - borderSize.x, borderSize.y);

		// bottom 4
		math::Vector2 E = A + math::Vector2(borderSize.x, size.y - borderSize.y);
		math::Vector2 F = A + math::Vector2(0, size.y);
		math::Vector2 G = A + size - borderSize;
		math::Vector2 H = A + size;

		// apply rotation if needed
		if (rotation != 0) {
			math::Vector2* points[] = {&A, &B, &C, &D, &E, &F, &G, &H};
			for (size_t i = 0; i < 8; i++){
				*points[i] = points[i]->rotateAroundOrigin(rotation, rotationOrigin);
			}
		}

		// push top and bottom vertices
		pushVertice(A, {0, 0}, col);
		pushVertice(B, {0, 0}, col);
		pushVertice(C, {0, 0}, col);
		pushVertice(D, {0, 0}, col);

		pushVertice(E, {0, 0}, col);
		pushVertice(F, {0, 0}, col);
		pushVertice(G, {0, 0}, col);
		pushVertice(H, {0, 0}, col);

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
		// TODO: Check if texture is transparent or not before applying this
		//glBlendFunc(tex.blend_src, tex.blend_dest);

		drawTexture(pos, size, tex.getHandle(), col, uvStart, uvEnd, rotation, origin);
	}

	void Stencil::drawTexture(mainframe::math::Vector2 pos, mainframe::math::Vector2 size, unsigned int rawTextureHandle, Color col, mainframe::math::Vector2 uvStart, mainframe::math::Vector2 uvEnd, float rotation, const mainframe::math::Vector2& origin) {
		if (col.a == 0) return;
		pos += offset;

		// TODO: idunno how to support clipping with rotation UV magic
		// move to shader? should also speed up this process here as the GPU already computes the coords for us
		// needs fallback for EGL then tho....

		if (rotation == 0 && !clipDisabled) {
			auto& clip = getClippingRect();
			auto pixelSize = (uvEnd - uvStart) / size;

			if (pos.x > clip.pos.x + clip.size.x) return;
			if (pos.y > clip.pos.y + clip.size.y) return;
			if (pos.x + size.x < clip.pos.x) return;
			if (pos.y + size.y < clip.pos.y) return;

			if (pos.x < clip.pos.x) {
				float diff = clip.pos.x - pos.x;

				uvStart.x += diff * pixelSize.x;
				size.x -= diff;
				pos.x = clip.pos.x;
			}

			if (pos.y < clip.pos.y) {
				float diff = clip.pos.y - pos.y;

				uvStart.y += diff * pixelSize.y;
				size.y -= diff;
				pos.y = clip.pos.y;
			}

			float maxX = (clip.pos.x + clip.size.x);
			float maxY = (clip.pos.y + clip.size.y);

			if (pos.x + size.x > maxX) {
				float diff = pos.x + size.x - clip.pos.x - clip.size.x;
				size.x = maxX - pos.x;

				uvEnd.x -= diff * pixelSize.x;
			}

			if (pos.y + size.y > maxY) {
				float diff = pos.y + size.y - clip.pos.y - clip.size.y;
				size.y = maxY - pos.y;

				uvEnd.y -= diff * pixelSize.y;
			}
		}

		if (size == 0) return;

		math::Vector2 b = pos;
		math::Vector2 c = pos;

		b.x += size.x;
		c.y += size.y;

		setTexture(rawTextureHandle);
		if (shaderOverride) setShader(shader2D);

		auto rotOrigin = origin.isNaN() ? pos + size / 2 : pos + origin;

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

	void Stencil::drawLine(const mainframe::math::Vector2& from, const mainframe::math::Vector2& to, float width, const Color& col, const mainframe::math::Vector2 uvStart, const mainframe::math::Vector2 uvEnd) {
		setTexture(getPixelTexture());
		if (shaderOverride) setShader(shader2D);

		float angle = from.angle(to);

		auto vertA = from + mainframe::math::Vector2::cosSin(angle) * width;
		auto vertB = from + mainframe::math::Vector2::cosSin(angle) * -width;
		auto vertC = to + mainframe::math::Vector2::cosSin(angle) * width;
		auto vertD = to + mainframe::math::Vector2::cosSin(angle) * -width;

		pushVertice(vertA + offset, uvStart, col);
		pushVertice(vertB + offset, {uvEnd.x, uvStart.y}, col);
		pushVertice(vertC + offset, {uvStart.x, uvEnd.y}, col);
		pushVertice(vertD + offset, uvEnd, col);

		pushIndices(4, 3, 2);
		pushIndices(3, 2, 1);
	}

	void Stencil::drawBox(const math::Vector2& pos, const math::Vector2& size, Color col) {
		drawTexture(pos, size, getPixelTexture(), col);
	}

	void Stencil::drawBox(const math::Vector2& pos, const math::Vector2& size, Color col, float rotation) {
		drawTexture(pos, size, getPixelTexture(), col, {}, {1, 1}, rotation);
	}

	void Stencil::drawText(const Font& font, const std::string& text, const math::Vector2& pos, Color col, TextAlignment alignx, TextAlignment aligny, float rotation, const mainframe::math::Vector2& origin) {
		if (col.a == 0 || text.empty()) return;
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

		startpos.x = std::roundf(startpos.x);
		startpos.y = std::roundf(startpos.y);

		float lineheight = font.getLineHeight();
		startpos.y += lineheight + static_cast<float>(font.face->size->metrics.descender >> 6);
		math::Vector2 curpos = startpos;

		auto rotOrigin = origin.isNaN() ? pos + tsize / 2 + offset : origin;

		const Glyph* prevGlyph = nullptr;

		uint32_t point = 0;
		auto beginIter = text.begin();
		auto endIter = utf8::find_invalid(text.begin(), text.end());
		while (beginIter != endIter) {
			point = utf8::next(beginIter, endIter);

			if (point == '\n') {
				curpos.y += lineheight;
				curpos.x = startpos.x;

				prevGlyph = nullptr;
				continue;
			}

			if (!font.hasGlyph(point)) continue;
			auto& glyph = font.getGlyph(point);
			if (prevGlyph != nullptr) {
				curpos.x += font.getKerning(glyph, *prevGlyph);
			}

			auto oldState = getShaderOverride();
			setShaderOverride(false);
			drawTexture(
				{curpos.x + glyph.bearing.x, curpos.y - glyph.bearing.y},
				{static_cast<float>(glyph.size.x), static_cast<float>(glyph.size.y)},
				font.atlas.glTexture,
				col,
				glyph.textureTopLeft,
				glyph.textureBottomRight,
				rotation,
				origin
			);
			setShaderOverride(oldState);

			curpos.x += glyph.advance.x;
			curpos.y += glyph.advance.y;
			prevGlyph = &glyph;
		}
	}

	bool Stencil::getDisableDept() {
		return deptDisabled;
	}

	void Stencil::setDisableDept(bool disabled) {
		deptDisabled = disabled;
	}

	bool Stencil::getShaderOverride() {
		return shaderOverride;
	}

	void Stencil::setShaderOverride(bool enabled) {
		shaderOverride = enabled;
	}

	Stencil::~Stencil() {
		getPixelTextureDecRef();
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
			if (tposAttrib != -1) {
				glEnableVertexAttribArray(tposAttrib);
				glVertexAttribPointer(tposAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 3));
			}

			GLint colAttrib = glGetAttribLocation(handle, "color");
			if (colAttrib != -1) {
				glEnableVertexAttribArray(colAttrib);
				glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 5));
			}
		};

		shader2DText.attachRaw("\
		#version 300 es\n\
		precision mediump float;\n\
		out vec4 outColor;\n\n\
		in vec4 output_color;\n\
		in vec2 output_texpos;\n\n\
		uniform sampler2D tex;\n\n\
		void main(){\n\
			vec2 tex = texture2D(tex, output_texpos).rg;\n\
			vec4 col = vec4(0.0, 0.0, 0.0, tex.g) * output_color;\n\
			\n\
			if((tex.g != 0. && tex.r != 0.) || tex.g == 0.) col = vec4(1.0, 1.0, 1.0, tex.r) * output_color; \n\
			\n\
			outColor = col;\n\
		}\n", GL_FRAGMENT_SHADER);

		shader2DText.attachRaw("\
		#version 300 es\n\
		precision mediump float;\n\
		in vec3 position;\n\
		in vec2 texpos;\n\
		in vec4 color;\n\n\
		out vec2 output_texpos;\n\
		out vec4 output_color;\n\
		void main() {\n\
			gl_Position = vec4(position, 1.0);\n\
			output_color = color;\n\
			output_texpos = texpos;\n\
		}\n", GL_VERTEX_SHADER);

		initShader(shader2DText);

		shader2D.attachRaw("#version 300 es\nprecision mediump float;\nout vec4 outColor;\n\nin vec4 output_color;\nin vec2 output_texpos;\n\nuniform sampler2D tex;\n\nvoid main(){\nvec4 texColor = texture(tex, output_texpos) * output_color;\n\nif (texColor.a <= 0.0) discard;\noutColor=texColor;\n}\n", GL_FRAGMENT_SHADER);
		shader2D.attachRaw("#version 300 es\nprecision mediump float;\nin vec3 position;\nin vec2 texpos;\nin vec4 color;\n\nout vec2 output_texpos;\nout vec4 output_color;\nvoid main() {\ngl_Position = vec4(position, 1.0);\noutput_color = color;\noutput_texpos = texpos;\n}\n", GL_VERTEX_SHADER);
		initShader(shader2D);
#endif

		getPixelTextureIncRef();
		auto& pixel = getPixelTexture();
		if (pixel.getHandle() == -1) {
			pixel.upload();
		}

		// filled in later by windowSize
		clippingRects.emplace_back(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void Stencil::pushClipping(const mainframe::math::AABB& rect) {
		mainframe::math::AABB newrect = {rect.pos.x + offset.x, rect.pos.y + offset.y, rect.size.x, rect.size.y};
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
		r.size.x = static_cast<float>(windowSize.x);
		r.size.y = static_cast<float>(windowSize.y);
	}

	const mainframe::math::Vector2i& Stencil::getWindowSize() const {
		return windowSize;
	}


	void Stencil::recordStart(bool supressDraw) {
		draw();
		recordings.push_back(std::make_shared<Recording>(supressDraw));
	}

	std::shared_ptr<Stencil::Recording> Stencil::recordStop() {
		if (recordings.empty()) throw std::runtime_error("no recordings active");

		draw();

		auto ret = recordings.back();
		recordings.pop_back();

		return ret;
	}

	void Stencil::drawRecording(const Recording& recording) {
		for (auto& chunk : recording.chunks) {
			setShader(chunk.shader);
			setTexture(chunk.texture);

			auto verticeOffset = static_cast<unsigned int>(vertices.size());
			vertices.insert(vertices.end(), chunk.vertices.begin(), chunk.vertices.end());

			size_t incideOffset = indices.size();
			indices.insert(indices.end(), chunk.indices.begin(), chunk.indices.end());

			for (size_t i = incideOffset, j = indices.size(); i < j; i++) {
				indices[i] += verticeOffset;
			}
		}
	}

	void Stencil::drawRecording(const Recording& recording, const mainframe::math::Vector2& pos) {
		for (auto& chunk : recording.chunks) {
			setShader(chunk.shader);
			setTexture(chunk.texture);

			auto posOffset = pos / windowSize;
			posOffset.y *= -1;

			size_t verticeOffset = vertices.size();
			vertices.insert(vertices.end(), chunk.vertices.begin(), chunk.vertices.end());
			for (size_t i = verticeOffset, j = vertices.size(); i < j; i++) {
				vertices[i].x = (((vertices[i].x + 1) / 2) + posOffset.x) * 2 - 1;
				vertices[i].y = (((vertices[i].y - 1) / 2 + 1.0f) + posOffset.y) * 2 - 1;
			}

			size_t incideOffset = indices.size();
			indices.insert(indices.end(), chunk.indices.begin(), chunk.indices.end());

			for (size_t i = incideOffset, j = indices.size(); i < j; i++) {
				indices[i] += static_cast<unsigned int>(verticeOffset);
			}
		}
	}

	void Stencil::drawRecording(const Recording& recording, const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size) {
		for (auto& chunk : recording.chunks) {
			setShader(chunk.shader);
			setTexture(chunk.texture);

			auto posOffset = pos / windowSize;
			posOffset.y *= -1;

			size_t verticeOffset = vertices.size();
			vertices.insert(vertices.end(), chunk.vertices.begin(), chunk.vertices.end());
			for (size_t i = verticeOffset, j = vertices.size(); i < j; i++) {
				vertices[i].x = (((vertices[i].x + 1) / 2 * size.x) + posOffset.x) * 2 - 1;
				vertices[i].y = (((vertices[i].y - 1) / 2 * size.y + 1.0f) + posOffset.y) * 2 - 1;
			}

			size_t indiceOffset = indices.size();
			indices.insert(indices.end(), chunk.indices.begin(), chunk.indices.end());

			for (size_t i = indiceOffset, j = indices.size(); i < j; i++) {
				indices[i] += static_cast<unsigned int>(verticeOffset);
			}
		}
	}

	void Stencil::drawRecording(const Recording& recording, const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size, float rotation, const mainframe::math::Vector2& origin) {
		auto orginPos = (pos + origin) / windowSize * 2 - 1;
		orginPos.y *= -1;

		for (auto& chunk : recording.chunks) {
			setShader(chunk.shader);
			setTexture(chunk.texture);

			auto posOffset = pos / windowSize;
			posOffset.y *= -1;

			size_t verticeOffset = vertices.size();
			vertices.insert(vertices.end(), chunk.vertices.begin(), chunk.vertices.end());
			for (size_t i = verticeOffset, j = vertices.size(); i < j; i++) {
				mainframe::math::Vector2 newpos = {
					(((vertices[i].x + 1) / 2 * size.x) + posOffset.x) * 2 - 1,
					(((vertices[i].y - 1) / 2 * size.y + 1.0f) + posOffset.y) * 2 - 1
				};

				auto rotated = newpos.rotateAroundOrigin(rotation, orginPos);

				vertices[i].x = rotated.x;
				vertices[i].y = rotated.y;
			}

			size_t indiceOffset = indices.size();
			indices.insert(indices.end(), chunk.indices.begin(), chunk.indices.end());

			for (size_t i = indiceOffset, j = indices.size(); i < j; i++) {
				indices[i] += static_cast<unsigned int>(verticeOffset);
			}
		}
	}

	void Stencil::draw() {
		if (vertices.empty()) return;

		bool suppressed = false;
		for (auto& recording : recordings) {
			if (recording->supressDraw) suppressed = true;

			recording->chunks.emplace_back(
				currentShaderHandle,
				currentTextureHandle,
				vertices,
				indices
			);
		}

		if (suppressed) {
			vertices.clear();
			indices.clear();
			return;
		}

		if (getDisableDept()) glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, currentTextureHandle);

#ifndef MAINFRAME_EGL
		glUseProgram(currentShaderHandle);

		glBindVertexArray(buffer.getVao());

		glBindBuffer(GL_ARRAY_BUFFER, buffer.getVbo());
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * 9 * sizeof(float), vertices.data(), GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.getEbo());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STREAM_DRAW);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
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
