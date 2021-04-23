#pragma once

#include <mainframe/ui/element.h>
#include <mainframe/ui/keystate.h>
#include <mainframe/ui/mousestate.h>
#include <mainframe/utils/event.hpp>
#include <vector>
#include <string>

namespace mainframe {
	namespace game {
		class Window;
	}

	namespace ui {
		class Scene : public ElementContainer {
		private:
			std::weak_ptr<Element> focusedElement;
			std::weak_ptr<Element> hoveredElement;

			void drawElm(Element& elm, render::Stencil& stencil);
			void updateElm(Element& elm);

			void mousePress(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods, MouseState state);
			void mouseScroll(const math::Vector2i& mousePos, const math::Vector2i& offset);
			void mouseMove(const math::Vector2i& mousePos);
			void keyPress(unsigned int key, unsigned int scancode, ModifierKey mods, KeyState state);
			void keyChar(unsigned int key);

			std::shared_ptr<Element> findElement(const math::Vector2i& mousePos);
			std::shared_ptr<Element> findElement(const math::Vector2i& mousePos, math::Vector2i& offsetOut);
			std::shared_ptr<Element> findElement(const std::shared_ptr<Element>& elmPtr, const math::Vector2i& mousePos, const math::Vector2i& offset, math::Vector2i& offsetOut);

			utils::ringbuffer<std::function<void()>> invokes = {64};

		public:
			mainframe::utils::Event<const math::Vector2i&, unsigned int, ModifierKey, MouseState> onMousePress;
			mainframe::utils::Event<const math::Vector2i&, const math::Vector2i&> onMouseScroll;
			mainframe::utils::Event<const math::Vector2i&> onMouseMove;
			mainframe::utils::Event<unsigned int, unsigned int, ModifierKey, KeyState> onKeyPress;
			mainframe::utils::Event<unsigned int> onKeyChar;

			utils::ringbuffer<std::function<void()>>& getInvoker();
			void invoke(std::function<void()> func);

			void draw(render::Stencil& stencil);
			void update();

			void setWindow(game::Window& window);

			std::shared_ptr<Element> getFocus();
			void setFocus(std::shared_ptr<Element> elm);

			static std::shared_ptr<Scene> create();
		};
	}
}