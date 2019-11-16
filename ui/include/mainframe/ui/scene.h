#pragma once

#include <mainframe/ui/element.h>
#include <vector>
#include <string>

namespace mainframe {
	namespace game {
		class Window;
	}

	namespace ui {
		class Scene : public ElementContainer {
		private:
			int mousePressCount = 0;
			std::weak_ptr<Element> focusedElement;
			std::weak_ptr<Element> hoveredElement;

			void drawElm(Element& elm, render::Stencil& stencil);
			void updateElm(Element& elm);

			void mousePress(const math::Vector2i& mousePos, unsigned int button, unsigned int mods, unsigned int pressed);
			void mouseScroll(const math::Vector2i& mousePos, const math::Vector2i& offset);
			void mouseMove(const math::Vector2i& mousePos);
			void keyPress(unsigned int key, unsigned int scancode, unsigned int mods, unsigned int action);
			void keyChar(unsigned int key);

			std::shared_ptr<Element> findElement(const math::Vector2i& mousePos);
			std::shared_ptr<Element> findElement(const math::Vector2i& mousePos, math::Vector2i& offsetOut);
			std::shared_ptr<Element> findElement(const std::shared_ptr<Element>& elmPtr, const math::Vector2i& mousePos, const math::Vector2i& offset, math::Vector2i& offsetOut);

		public:
			void draw(render::Stencil& stencil);
			void update();

			void setWindow(game::Window& window);

			static std::shared_ptr<Scene> create();
		};
	}
}