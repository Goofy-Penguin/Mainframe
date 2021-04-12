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
			for (auto& ent : entities) {
				ent->update();
			}
		}
	}
}
