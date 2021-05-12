#include <mainframe/game/world.h>

namespace mainframe {
	namespace game {
		void World::removeEntity(size_t id) {
			for (size_t i = 0; i < entities.size(); i++) {
				auto& ent = entities[i];
				if (ent->getId() != id) continue;

				entities.erase(entities.begin() + i);
			}
		}

		void World::removeEntity(Entity* ent) {
			removeEntity(ent->getId());
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
