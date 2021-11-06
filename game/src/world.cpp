#include <mainframe/game/world.h>

namespace mainframe::game {
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

	void World::update(float deltaTime) {
		auto oldsize = entities.size();
		for (size_t i = 0; i < oldsize; i++) {
			auto& ent = entities[i];

			ent->update(deltaTime);

			if (entities.size() != oldsize) {
				i--;
				oldsize = entities.size();
			}
		}
	}

	void World::runEntityQueue(mainframe::game::EntityIdType id) {
		auto fnd = entityCMDQueue.find(id);
		if(fnd == entityCMDQueue.end()) return;

		for(auto& fnc: fnd->second) fnc(); // Run all queued commands
		fnd->second.clear();
	}

	void World::queueEntityCommand(mainframe::game::EntityIdType id, std::function<void()> callback) {
		auto fnd = entityCMDQueue.find(id);
		if(fnd == entityCMDQueue.end()) {
			entityCMDQueue[id] = {callback};
		} else {
			fnd->second.push_back(callback);
		}
	}
}

