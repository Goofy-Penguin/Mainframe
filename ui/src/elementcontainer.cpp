#include <mainframe/ui/elementcontainer.h>
#include <mainframe/ui/element.h>

namespace mainframe {
	namespace ui {
		void ElementContainer::remove(bool childs) {
			while (childs && !children.empty()) children.front()->remove();

			if (!hasParent()) return;

			auto p = parent.lock();
			auto& parentChilds = p->getChildren();
			auto sharedPtr = getRef<Element>();
			parentChilds.erase(std::find(parentChilds.begin(), parentChilds.end(), sharedPtr));

			parent.reset();
		}

		void ElementContainer::addChild(std::shared_ptr<Element>& elm) {
			elm->setParent(getRef<ElementContainer>());
		}

		void ElementContainer::setParent(std::shared_ptr<ElementContainer>& elm) {
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
			pos = pos_;
		}

		void ElementContainer::setSize(const math::Vector2i& size_) {
			size = size_;
		}

		const math::Vector2i& ElementContainer::getPos() const {
			return pos;
		}

		const math::Vector2i& ElementContainer::getSize() const {
			return size;
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