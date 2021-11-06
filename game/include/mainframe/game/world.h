#pragma once

#include <mainframe/game/entity.h>
#include <vector>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <functional>

namespace mainframe::game {

	class World {
		std::vector<std::unique_ptr<Entity>> entities;
		std::unordered_map<mainframe::game::EntityIdType, std::vector<std::function<void()>>> entityCMDQueue;

	public:
		virtual ~World() = default;

		virtual void queueEntityCommand(mainframe::game::EntityIdType id, std::function<void()> callback);
		virtual void runEntityQueue(mainframe::game::EntityIdType id);

		std::unique_ptr<Entity> removeEntity(Entity* ent);
		std::unique_ptr<Entity> removeEntity(size_t id);

		virtual void update(float deltaTime);

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
		const std::vector<T*> getEntitiesInRange(const mainframe::math::Vector3& pos, float maxdistance, float mindistance = -1) {
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
			ent->setWorld(this);

			T& retEnt = *ent;
			entities.push_back(std::move(ent));

			return retEnt;
		}

		template<class T>
		void addEntity(std::unique_ptr<T>&& ent) {
			ent->setWorld(this);
			entities.push_back(std::move(ent));
		}

		bool hasEntity(mainframe::game::EntityIdType id) const {
			for (auto& ent : entities) {
				if (ent->getId() == id) {
					return true;
				}
			}

			return false;
		}

		template<class T = Entity>
		T& findEntity(mainframe::game::EntityIdType id) const {
			for (auto& ent : entities) {
				if (ent->getId() == id) {
					return *dynamic_cast<T*>(ent.get());
				}
			}

			throw std::runtime_error("no entity by id found in entitites list");
		}
	};
}
