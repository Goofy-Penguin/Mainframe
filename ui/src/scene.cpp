#include <mainframe/ui/scene.h>
#include <mainframe/game/window.h>

using namespace mainframe::game;

namespace mainframe {
	namespace ui {
		std::shared_ptr<Scene> Scene::create() {
			auto ret = std::make_shared<Scene>();
			ret->setRef(ret);
			return ret;
		}

		std::shared_ptr<Element> Scene::getFocus() {
			if (focusedElement.expired()) return {};
			return focusedElement.lock();
		}

		void Scene::setFocus(std::shared_ptr<Element> elm) {
			focusedElement = elm;
		}

		void Scene::setWindow(game::Window& window) {
			setPos(0);
			setSize(window.getSize());

			window.addOnChar([this](Window& win, unsigned int character) mutable {
				keyChar(character);
			});

			window.addOnKey([this](Window& win, unsigned int key, unsigned int scancode, unsigned int action, unsigned int mods) mutable {
				keyPress(key, scancode, static_cast<ModifierKey>(mods), static_cast<KeyState>(action));
			});

			window.addOnMouseKey([this](Window& win, const math::Vector2i& location, unsigned int button, unsigned int action, unsigned int mods) mutable {
				mousePress(location, button, static_cast<ModifierKey>(mods), static_cast<MouseState>(action));
			});

			window.addOnMouseMove([this](Window& win, const math::Vector2i& location) mutable {
				mouseMove(location);
			});

			window.addOnScroll([this](Window& win, const math::Vector2i& location, const math::Vector2i& offset) mutable {
				mouseScroll(location, offset);
			});
		}

		void Scene::drawElm(Element& elm, render::Stencil& stencil) {
			if (elm.isHidden()) return;

			stencil.pushOffset(elm.getPos());
			stencil.pushClipping({{0, 0}, elm.getSize()});

			elm.drawBefore(stencil);
			elm.draw(stencil);

			auto elms = elm.getChildren();
			for (auto& elm : elms) {
				drawElm(*elm, stencil);
			}
			elm.drawAfter(stencil);

			stencil.popClipping();
			stencil.popOffset();
		}

		void Scene::updateElm(Element& elm) {
			auto& invoker = elm.getInvoker();
			while (invoker.available()) {
				invoker.pop()();
			}

			elm.updateBefore();
			elm.update();

			auto elms = elm.getChildren();
			for (auto& elm : elms) {
				updateElm(*elm);
			}

			elm.updateAfter();
		}

		void Scene::draw(render::Stencil& stencil) {
			auto elms = getChildren();
			for (auto& elm : elms) {
				drawElm(*elm, stencil);
			}

			stencil.draw();
		}

		void Scene::update() {
			auto& invoker = getInvoker();
			while (invoker.available()) {
				invoker.pop()();
			}

			auto elms = getChildren();
			for (auto elm : elms) {
				updateElm(*elm);
			}
		}

		utils::ringbuffer<std::function<void()>>& Scene::getInvoker() {
			return invokes;
		}

		void Scene::invoke(std::function<void()> func) {
			invokes.push(func);
		}

		std::shared_ptr<Element> Scene::findElement(const math::Vector2i& mousePos) {
			math::Vector2i offsetOut;
			auto elms = getChildren();
			for (auto& elmPtr : elms) {
				auto child = findElement(elmPtr, mousePos, {}, offsetOut);
				if (child != nullptr) return child;
			}

			return nullptr;
		}

		std::shared_ptr<Element> Scene::findElement(const math::Vector2i& mousePos, math::Vector2i& offsetOut) {
			auto elms = getChildren();

			size_t count = elms.size();
			if (count == 0) return nullptr;

			for (size_t i = count; i > 0; i--) {
				auto child = findElement(elms[i - 1], mousePos, {}, offsetOut);

				if (child != nullptr) {
					return child;
				}
			}

			return nullptr;
		}

		std::shared_ptr<Element> Scene::findElement(const std::shared_ptr<Element>& elmPtr, const math::Vector2i& mousePos, const math::Vector2i& offset, math::Vector2i& offsetOut) {
			auto& elm = *elmPtr;
			if (elm.isHidden()) return nullptr;
			if (!elm.hitTest(mousePos)) return nullptr;

			auto& pos = elm.getPos();

			auto elms = elm.getChildren();
			size_t count = elms.size();
			for (size_t i = count; i > 0; i--) {
				auto found = findElement(elms[i - 1], mousePos - pos, offset + pos, offsetOut);
				if (found != nullptr) return found;
			}

			offsetOut = offset + pos;
			return elmPtr;
		}

		void Scene::mousePress(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods, MouseState state) {
			if (state == MouseState::inactive) {
				if (focusedElement.expired()) {
					onMousePress(mousePos, button, mods, state);
					return;
				}

				auto focused = focusedElement.lock();
				if (focused == nullptr) {
					onMousePress(mousePos, button, mods, state);
					return;
				}

				focused->mouseUp(mousePos - focused->getPos(), button, mods);
				focused->onMouseUp(mousePos - focused->getPos(), button, mods);
				return;
			}

			math::Vector2i offsetOut;
			auto target = findElement(mousePos, offsetOut);
			if (target == nullptr) {
				focusedElement.reset();

				onMousePress(mousePos, button, mods, state);
				return;
			}

			if (!focusedElement.expired()) focusedElement.lock()->setFocused(false);

			target->setFocused(true);
			focusedElement = target;

			target->mouseDown(mousePos - offsetOut, button, mods);
			target->onMouseDown(mousePos - offsetOut, button, mods);
		}

		void Scene::mouseMove(const math::Vector2i& mousePos) {
			math::Vector2i offsetOut;
			auto target = findElement(mousePos, offsetOut);

			// not hovering anything, so send it off to the scene event
			if (target == nullptr) {
				// do we have previous element
				if (!hoveredElement.expired()) {
					auto elm = hoveredElement.lock();
					elm->setHovering(false);

					hoveredElement.reset();
				}

				onMouseMove(mousePos);
				return;
			}

			// see if we're having a different target, if so notify it
			if (!hoveredElement.expired()) {
				auto elm = hoveredElement.lock();
				if (elm != target) {
					elm->setHovering(false);
					target->setHovering(true);
					hoveredElement = target;
				}
			} else {
				target->setHovering(true);
				hoveredElement = target;
			}

			target->mouseMove(mousePos - offsetOut);
			target->onMouseMove(mousePos - offsetOut);
		}

		void Scene::mouseScroll(const math::Vector2i& mousePos, const math::Vector2i& offset) {
			if (!hoveredElement.expired()) {
				auto elm = hoveredElement.lock();
				elm->mouseScroll(mousePos, offset);

				return;
			}

			onMouseScroll(mousePos, offset);
		}

		void Scene::keyPress(unsigned int key, unsigned int scancode, ModifierKey mods, KeyState state) {
			if (focusedElement.expired()) {
				onKeyPress(key, scancode, mods, state);
				return;
			}

			auto elm = focusedElement.lock();
			switch (state) {
				case KeyState::inactive: {
					elm->keyDown(key, scancode, mods, false);
					elm->onKeyDown(key, scancode, mods, false);
					break;
				}

				case KeyState::active: {
					elm->keyUp(key, scancode, mods);
					elm->onKeyUp(key, scancode, mods);
					break;
				}

				case KeyState::repeating: {
					elm->keyDown(key, scancode, mods, true);
					elm->onKeyDown(key, scancode, mods, true);
					break;
				}

				default:
					throw std::runtime_error("invalid action for keypress");
			}
		}

		void Scene::keyChar(unsigned int key) {
			if (focusedElement.expired()) {
				onKeyChar(key);
				return;
			}

			auto elm = focusedElement.lock();
			elm->keyChar(key);
			elm->onKeyChar(key);
		}
	}
}