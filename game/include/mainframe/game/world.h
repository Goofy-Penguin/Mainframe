#pragma once

#include <mainframe/game/entity.h>
#include <vector>
#include <memory>

namespace mainframe {
	namespace game {
		class World {
			std::vector<std::shared_ptr<Entity>> entities;

		public:
			template<class T, class... Args>
			Entity& createEntity(Args... args) {
				auto ent = std::make_shared(*this, args...);
				ent->generateUniqueId();
				ent->setReference(ent);

				entities.push_back(ent);
				return *ent;
			}

			template<class T>
			std::shared_ptr<T> findEntity(size_t id) const {
				for (auto& ent : entities) {
					if (ent->getId() == id) {
						return std::reinterpret_pointer_cast<T>(ent);
					}
				}

				return {};
			}

			void removeEntity(Entity* ent);
			void removeEntity(size_t id);
		};
	}
}
