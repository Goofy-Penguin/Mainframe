#include <mainframe/ui/element.h>

namespace mainframe {
	namespace ui {
		const std::string& Element::getName() const {
			return name;
		}

		void Element::setName(const std::string& name_) {
			name = name_;
		}

		bool Element::getHovering() const {
			return hovering;
		}

		utils::ringbuffer<std::function<void()>>& Element::getInvoker() {
			return invokes;
		}

		void Element::invoke(std::function<void()> func) {
			invokes.push(func);
		}

		void Element::setHovering(bool hovering_) {
			hovering = hovering_;
		}

		math::Vector2i Element::getPosAbsolute() const {
			math::Vector2i ret;

			std::shared_ptr<Element> parent = getRef<Element>();
			while ((parent = parent->getParent<Element>()) != nullptr) {
				ret += parent->getPos();
			}

			return ret + getPos();
		}

		void Element::drawBefore(render::Stencil& stencil) {}
		void Element::draw(render::Stencil& stencil) {}
		void Element::drawAfter(render::Stencil& stencil) {}

		void Element::updateBefore() {}
		void Element::update() {}
		void Element::updateAfter() {}

		void Element::show() { hidden = false; }
		void Element::hide() { hidden = true; }
		bool Element::isHidden() { return hidden; }

		bool Element::isfocused() { return focused; }
		void Element::setFocused(bool focused_) {
			focused = focused_;
			onFocusChange(focused_);
		}

		bool Element::hitTest(const math::Vector2i& mousePos) {
			return getAABB().contains(mousePos);
		}

		void Element::initialize() {}
		void Element::mouseDown(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods) {}
		void Element::mouseUp(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods) {}
		void Element::mouseScroll(const math::Vector2i& mousePos, const math::Vector2i& offset) {}
		void Element::mouseMove(const math::Vector2i& mousePos) {}
		void Element::keyDown(unsigned int key, unsigned int scancode, ModifierKey mods, bool repeating) {}
		void Element::keyUp(unsigned int key, unsigned int scancode, ModifierKey mods) {}
		void Element::keyChar(unsigned int key) {}
	}
}