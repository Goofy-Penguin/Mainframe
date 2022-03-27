#pragma once

#include <mainframe/math/vector2.h>
#include <mainframe/render/color.h>
#include <mainframe/render/stencil.h>
#include <mainframe/utils/ringbuffer.hpp>
#include <mainframe/ui/elementcontainer.h>
#include <mainframe/ui/modifierkey.h>
#include <mainframe/utils/event.hpp>
#include <memory>
#include <string>
#include <functional>

namespace mainframe {
	namespace ui {
		class Element : public ElementContainer {
		private:
			std::string name;
			bool hovering = false;
			bool hidden = false;
			bool focused = false;
			utils::ringbuffer<std::function<void()>> invokes = {64};

		public:
			mainframe::utils::Event<const math::Vector2i&, unsigned int, ModifierKey> onMouseDown;
			mainframe::utils::Event<const math::Vector2i&, unsigned int, ModifierKey> onMouseUp;
			mainframe::utils::Event<const math::Vector2i&, const math::Vector2i&> onMouseScroll;
			mainframe::utils::Event<const math::Vector2i&> onMouseMove;
			mainframe::utils::Event<unsigned int, unsigned int, ModifierKey, bool> onKeyDown;
			mainframe::utils::Event<unsigned int, unsigned int, ModifierKey> onKeyUp;
			mainframe::utils::Event<unsigned int> onKeyChar;
			mainframe::utils::Event<bool> onFocusChange;

			math::Vector2i getPosAbsolute() const;
			const std::string& getName() const;
			bool getHovering() const;

			void setName(const std::string& name_);
			virtual void setHovering(bool hovering_);

			utils::ringbuffer<std::function<void()>>& getInvoker();

			virtual void drawBefore(render::Stencil& stencil);
			virtual void draw(render::Stencil& stencil);
			virtual void drawAfter(render::Stencil& stencil);
			virtual void updateBefore();
			virtual void update();
			virtual void updateAfter();

			virtual bool lockKeyboard();
			virtual bool lockScroll();

			virtual void bringToFront();

			virtual void show();
			virtual void hide();
			bool isHidden();

			bool isfocused();
			virtual void setFocused(bool focused_);

			void invoke(std::function<void()> func);

			virtual bool hitTest(const math::Vector2i& mousePos);
			virtual void mouseDown(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods);
			virtual void mouseUp(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods);
			virtual void mouseScroll(const math::Vector2i& mousePos, const math::Vector2i& offset);
			virtual void mouseMove(const math::Vector2i& mousePos);
			virtual void keyDown(unsigned int key, unsigned int scancode, ModifierKey mods, bool repeating);
			virtual void keyUp(unsigned int key, unsigned int scancode, ModifierKey mods);
			virtual void keyChar(unsigned int key);
		};
	}
}
