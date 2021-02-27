#include <mainframe/ui/elementcontainer.h>
#include <mainframe/ui/element.h>

namespace mainframe {
	namespace ui {
		ElementContainer::ElementContainer() : threadId(std::this_thread::get_id()) {}

		std::thread::id ElementContainer::getThreadId() {
			return threadId;
		}

		void ElementContainer::remove(bool childs) {
			while (childs && !children.empty()) children.front()->remove();

			if (!hasParent()) {
				onRemoved();
				return;
			}

			auto p = parent.lock();
			auto& parentChilds = p->getChildren();
			auto sharedPtr = getRef<Element>();
			parentChilds.erase(std::find(parentChilds.begin(), parentChilds.end(), sharedPtr));

			parent.reset();
			onRemoved();
		}

		void ElementContainer::clearChildren() {
			while (!children.empty()) {
				children.front()->remove();
			}
		}

		void ElementContainer::addChild(const std::shared_ptr<Element>& elm) {
			elm->setParent(getRef<ElementContainer>());
		}

		std::shared_ptr<Element> ElementContainer::findChildElm(const std::string& name, bool recursive) const {
			for (auto& c : children) {
				if (c->getName() != name) {
					if (!recursive) continue;

					auto recRet = c->findChildElm(name, true);
					if (recRet == nullptr) continue;

					return recRet;
				}

				return c;
			}

			return nullptr;
		}

		void ElementContainer::setParent(const std::shared_ptr<ElementContainer>& elm) {
			auto sharedPtr = getRef<Element>();

			if (hasParent()) {
				auto& childs = parent.lock()->getChildren();
				childs.erase(std::find(childs.begin(), childs.end(), sharedPtr));
			}

			parent = elm;
			elm->getChildren().push_back(sharedPtr);
		}

		bool ElementContainer::hasParent() const {
			return !parent.expired();
		}

		void ElementContainer::setPos(const math::Vector2i& pos_) {
			aabb.pos = pos_;
		}

		void ElementContainer::setSize(const math::Vector2i& size_) {
			aabb.size = size_;
		}

		void ElementContainer::setAABB(const math::AABBi& aabb_) {
			setPos(aabb_.pos);
			setSize(aabb_.size);
		}

		const math::Vector2i& ElementContainer::getPos() const {
			return aabb.pos;
		}

		const math::Vector2i& ElementContainer::getSize() const {
			return aabb.size;
		}

		const math::AABBi& ElementContainer::getAABB() const {
			return aabb;
		}

		const std::vector<std::shared_ptr<Element>>& ElementContainer::getChildren() const {
			return children;
		}

		std::vector<std::shared_ptr<Element>>& ElementContainer::getChildren() {
			return children;
		}

		void ElementContainer::setRef(const std::shared_ptr<ElementContainer>& ref_) {
			ref = ref_;
		}
	}
}