#pragma once

#include <mainframe/game/entity.h>
#include <vector>
#include <memory>

namespace mainframe {
	namespace game {
		class World {
			std::vector<std::shared_ptr<Entity>> entities;

		public:
			virtual ~World() = default;

			void removeEntity(Entity* ent);
			void removeEntity(size_t id);

			template<class T = Entity>
			const std::vector<std::shared_ptr<T>> getEntities() {
				std::vector<std::shared_ptr<T>> ret;
				for (auto& ent : entities) {
					ret.push_back(std::dynamic_pointer_cast<T>(ent));
				}

				return ret;
			}

			template<class T, class WType, class... Args>
			T& createEntity(Args... args) {
				auto ent = std::make_shared<T>(dynamic_cast<WType*>(this), args...);
				ent->generateUniqueId();

				entities.push_back(ent);
				return *ent;
			}

			template<class T>
			std::shared_ptr<T> findEntity(size_t id) const {
				for (auto& ent : entities) {
					if (ent->getId() == id) {
						return std::dynamic_pointer_cast<T>(ent);
					}
				}

				return {};
			}
		};
	}
}
