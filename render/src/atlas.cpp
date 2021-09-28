#include <mainframe/render/atlas.h>

#include <utility>
#include <stdexcept>
#include <fmt/format.h>

namespace mainframe {
	namespace render {
		Atlas::Atlas() {
			glGenTextures(1, &glTexture);
			glBindTexture(GL_TEXTURE_2D, glTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_FLOAT, nullptr);
		}

		Atlas::~Atlas() {
			Release();
		}

		void Atlas::Release() {
			glDeleteTextures(1, &glTexture);
			glTexture = 0;
		}

		Atlas::Atlas(Atlas&& other) : glTexture(other.glTexture) {
			other.glTexture = 0;
		}

		Atlas& Atlas::operator=(Atlas&& other) {
			if (this == &other) return *this;

			Release();
			std::swap(glTexture, other.glTexture);
			return *this;
		}

		size_t Atlas::getSpriteCount() const {
			return spriteCount;
		}

		AtlasNode& Atlas::addSprite(int width, int height) {
			auto nodeOpt = rootNode.InsertNode(width, height);
			if (!nodeOpt.has_value()) {
				throw std::runtime_error(fmt::format("Error: failed to add sprite with size {}, {}", width, height));
			}

			spriteCount++;

			auto& node = (*nodeOpt).get();
			return node;
		}

		std::optional<std::reference_wrapper<AtlasNode>> AtlasNode::InsertNode(int insertedWidth, int insertedHeight) {
			if (!empty) {
				//already full, bad luck
				return std::nullopt;
			}

			if (left && right) {
				// both children exist, which means this node is full, try left then right
				if (auto node = left->InsertNode(insertedWidth, insertedHeight); node != std::nullopt) {
					return node;
				}
				if (auto node = right->InsertNode(insertedWidth, insertedHeight); node != std::nullopt) {
					return node;
				}
				return std::nullopt;
			}

			if (insertedWidth > width || insertedHeight > height) {
				// to be insterted node is too big, can't fit here
				return std::nullopt;
			}

			if (width == insertedWidth && height == insertedHeight) {
				// fits perfectly
				empty = false;
				return *this;
			}

			// if all of the above didn't return, the current leaf is large enough,
			// with some space to spare, so we split up the current node so we have
			// one prefectly fitted node and some spare nodes
			int remainingWidth = width - insertedWidth;
			int remainingHeight = height - insertedHeight;

			bool isRemainderWiderThanHigh = remainingWidth > remainingHeight;

			if (isRemainderWiderThanHigh) { // if wider than high, split verticallly
				left  = std::unique_ptr<AtlasNode>(new AtlasNode{x,                 y, insertedWidth,  height, true, nullptr, nullptr});
				right = std::unique_ptr<AtlasNode>(new AtlasNode{x + insertedWidth, y, remainingWidth, height, true, nullptr, nullptr});
			} else { // That'd make the remainder higher than it's wide, split horizontally
				left  = std::unique_ptr<AtlasNode>(new AtlasNode{x, y,                  width, insertedHeight,  true, nullptr, nullptr});
				right = std::unique_ptr<AtlasNode>(new AtlasNode{x, y + insertedHeight, width, remainingHeight, true, nullptr, nullptr});
			}

			return left->InsertNode(insertedWidth, insertedHeight);
		}
	}
}