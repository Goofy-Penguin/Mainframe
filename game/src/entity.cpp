#include <mainframe/game/entity.h>
#include <mainframe/game/world.h>

namespace mainframe {
	namespace game {
		Entity::Entity(World& world_) : world(world_) {

		}

		Entity::~Entity() {

		}

		void Entity::remove() {
			world.removeEntity(this);
		}

		size_t Entity::getId() const {
			return id;
		}
	}
}
