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

		void Scene::setWindow(game::Window& window) {
			setPos(0);
			setSize(window.getSize());

			window.addOnChar([this](Window& win, unsigned int character) mutable {
				keyChar(character);
			});

			window.addOnKey([this](Window& win, unsigned int key, unsigned int scancode, unsigned int action, unsigned int mods) mutable {
				keyPress(key, scancode, static_cast<ModifierKey>(mods), action);
			});

			window.addOnMouseKey([this](Window& win, const math::Vector2i& location, unsigned int button, unsigned int action, unsigned int mods) mutable {
				mousePress(location, button, static_cast<ModifierKey>(mods), action == 1);
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
			for (auto& elmPtr : elms) {
				auto child = findElement(elmPtr, mousePos, {}, offsetOut);
				if (child != nullptr) return child;
			}

			return nullptr;
		}

		std::shared_ptr<Element> Scene::findElement(const std::shared_ptr<Element>& elmPtr, const math::Vector2i& mousePos, const math::Vector2i& offset, math::Vector2i& offsetOut) {
			auto& elm = *elmPtr;
			if (elm.isHidden()) return nullptr;

			auto& pos = elm.getPos();
			auto& size = elm.getSize();

			if (mousePos.x < pos.x) return nullptr;
			if (mousePos.y < pos.y) return nullptr;
			if (mousePos.x >= pos.x + size.x) return nullptr;
			if (mousePos.y >= pos.y + size.y) return nullptr;

			auto elms = elm.getChildren();
			for (auto& child : elms) {
				auto found = findElement(child, mousePos - pos, offset + pos, offsetOut);
				if (found != nullptr) return found;
			}

			offsetOut = offset + pos;
			return elmPtr;
		}

		void Scene::mousePress(const math::Vector2i& mousePos, unsigned int button, ModifierKey mods, bool pressed) {
			if (!pressed) {
				mousePressCount--;

				if (focusedElement.expired()) {
					onMousePress(mousePos, button, mods, pressed);
					return;
				}

				auto focused = focusedElement.lock();
				if (focused == nullptr) {
					onMousePress(mousePos, button, mods, pressed);
					return;
				}

				focused->mouseUp(mousePos, button, mods);
				return;
			}

			mousePressCount++;

			math::Vector2i offsetOut;
			auto target = findElement(mousePos, offsetOut);
			if (target == nullptr) {
				focusedElement.reset();

				onMousePress(mousePos, button, mods, pressed);
				return;
			}

			if (!focusedElement.expired()) focusedElement.lock()->setfocused(false);

			target->setfocused(true);
			focusedElement = target;

			target->mouseDown(mousePos - offsetOut, button, mods);
		}

		void Scene::mouseMove(const math::Vector2i& mousePos) {
			math::Vector2i offsetOut;
			auto target = findElement(mousePos, offsetOut);

			if (!hoveredElement.expired()) {
				auto elm = hoveredElement.lock();
				elm->setHovering(false);
			}

			if (target == nullptr) {
				hoveredElement.reset();
			} else {
				hoveredElement = target;
				target->setHovering(true);
			}

			if (mousePressCount > 0 && !focusedElement.expired()) {
				auto elm = focusedElement.lock();
				elm->mouseMove(mousePos - elm->getPosAbsolute());
				return;
			}

			if (target == nullptr) {
				onMouseMove(mousePos);
				return;
			}

			target->mouseMove(mousePos - offsetOut);
		}

		void Scene::mouseScroll(const math::Vector2i& mousePos, const math::Vector2i& offset) {
			// TODO: element logic for scrolling

			onMouseScroll(mousePos, offset);
		}

		void Scene::keyPress(unsigned int key, unsigned int scancode, ModifierKey mods, unsigned int action) {
			if (focusedElement.expired()) {
				onKeyPress(key, scancode, mods, action);
				return;
			}

			auto elm = focusedElement.lock();
			switch (action) {
				case 0: {
					elm->keyDown(key, mods, false);
					break;
				}

				case 1: {
					elm->keyUp(key, mods);
					break;
				}

				case 2: {
					elm->keyDown(key, mods, true);
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
		}
	}
}