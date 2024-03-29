#pragma once

#include <mainframe/math/vector2.h>
#include <mainframe/math/aabb.h>
#include <mainframe/utils/event.hpp>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <stdexcept>

namespace mainframe {
	namespace ui {
		class Element;

		class ElementContainer {
			std::weak_ptr<ElementContainer> parent;
			std::weak_ptr<ElementContainer> ref;
			std::vector<std::shared_ptr<Element>> children;
			math::AABBi aabb;
			std::thread::id threadId;

			std::shared_ptr<Element> findChildElm(const std::string& name, bool recursive = false) const;

		public:
			mainframe::utils::Event<> onRemoved;
			ElementContainer();

			std::thread::id getThreadId();

			virtual void remove(bool childs = true);
			virtual void addChild(const std::shared_ptr<Element>& elm);
			virtual void setParent(const std::shared_ptr<ElementContainer>& elm);
			virtual bool hasParent() const;

			const math::Vector2i& getPos() const;
			const math::Vector2i& getSize() const;
			const math::AABBi& getAABB() const;

			virtual void setPos(const math::Vector2i& pos_);
			virtual void setSize(const math::Vector2i& size_);
			virtual void setAABB(const math::AABBi& aabb_);

			void clearChildren();

			void setRef(const std::shared_ptr<ElementContainer>& ref_);

			std::vector<std::shared_ptr<Element>>& getChildren();
			const std::vector<std::shared_ptr<Element>>& getChildren() const;

			template<class ChildT>
			std::shared_ptr<ChildT> findChild(const std::string& name, bool recursive = false) const {
				return std::dynamic_pointer_cast<ChildT>(findChildElm(name, recursive));
			}

			template<class ChildT>
			std::shared_ptr<ChildT> getRef() const {
				return std::dynamic_pointer_cast<ChildT>(ref.lock());
			}

			template<class ChildT>
			std::shared_ptr<ChildT> getParent() const {
				return std::dynamic_pointer_cast<ChildT>(parent.lock());
			}

			template<class T, typename... CallbackArgs>
			std::shared_ptr<T> createChild(CallbackArgs&&... args) {
				auto child = std::make_shared<T>(std::forward<CallbackArgs>(args)...);
				child->setRef(child);
				addChild(std::dynamic_pointer_cast<Element>(child));

				if (threadId != child->threadId) {
					throw std::runtime_error("invalid thread");
				}

				return child;
			}
		};
	}
}
