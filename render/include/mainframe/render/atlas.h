#pragma once

#include <GL/glew.h>

#include <memory>
#include <optional>
#include <functional>
		
namespace mainframe {
	namespace render {
		struct Sprite {
			int x;
			int y;
			int width;
			int height;
		};

		struct AtlasNode {
			int x;
			int y;
			int width;
			int height;
			bool empty;
			std::unique_ptr<AtlasNode> left;
			std::unique_ptr<AtlasNode> right;

			std::optional<std::reference_wrapper<AtlasNode>> InsertNode(int width, int height);
		};

		class Atlas {
		// TODO: move code around so glTexture doesn't ahve to be public anymore,
		// probably by moving the texture to textface
		public:
			GLuint glTexture;
			AtlasNode rootNode{0, 0, 2048, 2048, true, nullptr, nullptr};
			
			void Release();

		public:
			Atlas();
			Atlas(const Atlas& other) = delete;
			Atlas(Atlas&& other);

			Atlas& operator=(const Atlas& other) = delete;
			Atlas& operator=(Atlas&& other);

			~Atlas();

			AtlasNode& addSprite(int width, int height);
		};
	}
}