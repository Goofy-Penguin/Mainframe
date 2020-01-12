#pragma once

#include <mainframe/math/vector2.h>
#include <mainframe/render/color.h>
#include <mainframe/render/stencil.h>
#include <mainframe/utils/ringbuffer.hpp>
#include <mainframe/ui/elementcontainer.h>
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
			utils::ringbuffer<std::function<void()>> invokes = {64};

		public:
			math::Vector2i getPosAbsolute() const;
			const std::string& getName() const;
			bool getHovering() const;

			void setName(const std::string& name_);
			void setHovering(bool hovering_);

			utils::ringbuffer<std::function<void()>>& getInvoker();

			virtual void drawBefore(render::Stencil& stencil);
			virtual void draw(render::Stencil& stencil);
			virtual void drawAfter(render::Stencil& stencil);
			virtual void updateBefore();
			virtual void update();
			virtual void updateAfter();

			virtual void show();
			virtual void hide();
			bool isHidden();

			void invoke(std::function<void()> func);

			virtual void mouseDown(const math::Vector2i& mousePos, unsigned int button, unsigned int mods);
			virtual void mouseUp(const math::Vector2i& mousePos, unsigned int button, unsigned int mods);
			virtual void mouseScroll(const math::Vector2i& mousePos, const math::Vector2i& offset);
			virtual void mouseMove(const math::Vector2i& mousePos);
			virtual void keyDown(unsigned int key, unsigned int mods, unsigned int ctrl, unsigned int mode);
			virtual void keyUp(unsigned int key, unsigned int mods, unsigned int ctrl, unsigned int mode);
			virtual void keyChar(unsigned int key);
		};
	}
}