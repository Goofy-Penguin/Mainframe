#pragma once

#include <mainframe/render/color.h>
#include <mainframe/render/shader.h>
#include <mainframe/render/viabuffer.h>
#include <mainframe/render/texture.h>
#include <mainframe/math/aabb.h>
#include <mainframe/numbers/pi.h>

// for lower openGL
//#define MAINFRAME_EGL

namespace mainframe::render {
	class Font;

	class Stencil {
	public:
		struct PolygonVertice {
			mainframe::math::Vector2 pos;
			mainframe::math::Vector2 uv;
			Color col;
		};

		struct Polygon {
			std::vector<PolygonVertice> verts;
			std::vector<unsigned int> indices;
		};

		enum class FlipMode {
			Vertical,
			Horizontal,
			Both,
			None
		};

		enum class TextAlignment {
			Left,
			Center,
			Right
		};

		class VerticeData {
		public:
			float x = 0;
			float y = 0;
			float z = 0;

			float u = -1;
			float v = -1;

			float r = 0;
			float g = 0;
			float b = 0;
			float a = 0;

			VerticeData() = default;
			VerticeData(float _x, float _y, float _z, float _u, float _v, float _r, float _g, float _b, float _a);
		};

		struct RecordingChunk {
			int shader;
			unsigned int texture;

			std::vector<VerticeData> vertices;
			std::vector<unsigned int> indices;
		};

		class Recording {
		public:
			bool supressDraw = false;
			std::vector<RecordingChunk> chunks;

			Recording(bool supressDraw_ = false) : supressDraw(supressDraw_) {};
		};

	private:

		VIABuffer buffer;

		static Texture& getPixelTexture();

		int currentShaderHandle = -1;
		unsigned int currentTextureHandle = 0xFFFFFFFF;

		std::vector<VerticeData> vertices;
		std::vector<unsigned int> indices;

		mainframe::math::Vector2i windowSize;

		bool deptDisabled = false;
		bool clipDisabled = false;
		bool shaderOverride = true;

		mainframe::math::Vector2 offset;
		mainframe::math::Vector2 oldOffset;

		std::vector<mainframe::math::Vector2> offsets;
		std::vector<mainframe::math::AABB> clippingRects;

		std::vector<std::shared_ptr<Recording>> recordings;

		void pushVertice(const mainframe::math::Vector2& pos, const mainframe::math::Vector2& uv, const Color& col);
		void pushIndices(unsigned int a, unsigned int b, unsigned int c);

	public:
		Shader shader2DText;
		Shader shader2D;

		void drawRecording(const Recording& recording);
		void drawRecording(const Recording& recording, const mainframe::math::Vector2& pos);
		void drawRecording(const Recording& recording, const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size);
		void drawRecording(const Recording& recording, const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size, float rotation, const mainframe::math::Vector2& origin = {});

		void drawPolygon(const Polygon& poly);
		void drawCircle(const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size, size_t roundness, Color col, float angleStart = 0, float maxAngle = numbers::pi<float> * 2);
		void drawCircleOutline(const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size, size_t roundness, float borderSize, Color col, float angleStart = 0, float maxAngle = numbers::pi<float> * 2);
		void drawBoxOutlined(mainframe::math::Vector2 pos, const mainframe::math::Vector2& size, const mainframe::math::Vector2& borderSize, Color col, float rotation = 0, const mainframe::math::Vector2& rotationOrigin = {});
		void drawLine(const mainframe::math::Vector2& from, const mainframe::math::Vector2& to, float width, const Color& col, const mainframe::math::Vector2 uvStart = {0, 0}, const mainframe::math::Vector2 uvEnd = {1, 1});
		void drawBox(const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size, Color col);
		void drawBox(const mainframe::math::Vector2& pos, const mainframe::math::Vector2& size, Color col, float rotation);
		void drawText(const Font& font, const std::string& text, const mainframe::math::Vector2& pos, Color col, TextAlignment alignx = TextAlignment::Left, TextAlignment aligny = TextAlignment::Left, float rotation = 0, const mainframe::math::Vector2& origin = {std::nanf(""), std::nanf("")});
		void drawTriangle(const mainframe::math::Vector2& a, const mainframe::math::Vector2& aUV, const Color& colA, const mainframe::math::Vector2& b, const mainframe::math::Vector2& bUV, const Color& colB, const mainframe::math::Vector2& c, const mainframe::math::Vector2& cUV, const Color& colC);
		void drawTexture(mainframe::math::Vector2 pos, mainframe::math::Vector2 size, const Texture& tex, Color col = Colors::White, mainframe::math::Vector2 uvStart = {0, 0}, mainframe::math::Vector2 uvEnd = {1, 1}, float rotation = 0, const mainframe::math::Vector2& origin = {std::nanf(""), std::nanf("")});
		void drawTexture(mainframe::math::Vector2 pos, mainframe::math::Vector2 size, unsigned int rawTextureHandle, Color col = Colors::White, mainframe::math::Vector2 uvStart = {0, 0}, mainframe::math::Vector2 uvEnd = {1, 1}, float rotation = 0, const mainframe::math::Vector2& origin = {std::nanf(""), std::nanf("")});

		void draw();

		void pushLocalOffset();
		void popLocalOffset();

		void pushDisableClip();
		void popDisableClip();

		void pushOffset(const mainframe::math::Vector2& offset);
		void popOffset();
		const mainframe::math::Vector2& getOffset() const;

		void setDisableDept(bool disabled);
		bool getDisableDept();

		void setShaderOverride(bool enabled);
		bool getShaderOverride();

		void setWindowSize(const mainframe::math::Vector2i& size);
		const mainframe::math::Vector2i& getWindowSize() const;

		void setTexture(unsigned int tex);
		void setTexture(const Texture& tex);
		unsigned int getTexture();

		void setShader(int shader);
		void setShader(const Shader& shader);
		int getShader();

		void pushClipping(const mainframe::math::AABB& rect);
		void popClipping();
		bool getClippingEnabled() const;
		const mainframe::math::AABB& getClippingRect() const;
		bool isClipped(const math::Vector2& pos);


		void getPixelTextureIncRef();
		void getPixelTextureDecRef();
		int& getPixelTextureGetRef();

		void recordStart(bool supressDraw);
		std::shared_ptr<Recording> recordStop();

		Stencil();
		~Stencil();
	};
}
