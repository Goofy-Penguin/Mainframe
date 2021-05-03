#pragma once

#include <mainframe/game/entity.h>
#include <vector>
#include <memory>
#include <stdexcept>

namespace mainframe {
	namespace game {
		class World {
			std::vector<std::unique_ptr<Entity>> entities;

		public:
			virtual ~World() = default;

			void removeEntity(Entity* ent);
			void removeEntity(size_t id);

			virtual void update();

			template<class T = Entity>
			const std::vector<T*> getEntities() {
				std::vector<T*> ret;
				for (auto& ent : entities) {
					auto ptr = dynamic_cast<T*>(ent.get());
					if (ptr == nullptr) continue;

					ret.push_back(ptr);
				}

				return ret;
			}

			template<class T = Entity>
			const std::vector<T*> getEntitiesInRange(const mainframe::math::Vector3& pos, float maxdistance, float mindistance = 0) {
				std::vector<T*> ret;

				for (auto& ent : entities) {
					auto ptr = dynamic_cast<T*>(ent.get());
					if (ptr == nullptr) continue;

					float dist = ptr->getPosition().distance(pos);
					if (dist <= mindistance || dist >= maxdistance) continue;

					ret.push_back(ptr);
				}

				return ret;
			}

			template<class T, class WType, class... Args>
			T& createEntity(Args&&... args) {
				auto ent = std::make_unique<T>(dynamic_cast<WType*>(this), std::forward<Args>(args)...);
				ent->generateUniqueId();

				T& retEnt = *ent;
				entities.push_back(std::move(ent));

				return retEnt;
			}

			bool hasEntity(size_t id) const {
				for (auto& ent : entities) {
					if (ent->getId() == id) {
						return true;
					}
				}

				return false;
			}

			template<class T>
			T& findEntity(size_t id) const {
				for (auto& ent : entities) {
					if (ent->getId() == id) {
						return *dynamic_cast<T*>(ent.get());
					}
				}

				throw std::runtime_error("no entity by id found in entitites list");
			}
		};
	}
}
