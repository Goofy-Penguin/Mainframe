#include <mainframe/game/world.h>

namespace mainframe {
	namespace game {
		std::unique_ptr<Entity> World::removeEntity(size_t id) {
			for (size_t i = 0; i < entities.size(); i++) {
				if (entities[i]->getId() != id) continue;

				auto ent = std::move(entities[i]);
				entities.erase(entities.begin() + i);

				return ent;
			}

			return nullptr;
		}

		std::unique_ptr<Entity> World::removeEntity(Entity* ent) {
			return std::move(removeEntity(ent->getId()));
		}

		void World::update() {
			auto oldsize = entities.size();
			for (size_t i = 0; i < oldsize; i++) {
				auto& ent = entities[i];

				ent->update();

				if (entities.size() != oldsize) {
					i--;
					oldsize = entities.size();
				}
			}
		}
	}
}
